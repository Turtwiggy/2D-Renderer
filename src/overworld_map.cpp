#include "overworld_map.hpp"
#include "entity_common.hpp"
#include "pathfinding.hpp"

entt::entity create_overworld_unit(entt::registry& registry, sprite_handle handle, world_transform transform)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<overworld_tag>(res, overworld_tag());
    registry.assign<render_descriptor>(res, desc);

    return res;
}

entt::entity create_overworld_building(entt::registry& registry, const sprite_handle& handle, const world_transform& transform)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<overworld_tag>(res, overworld_tag());
    registry.assign<render_descriptor>(res, desc);

    return res;
}

std::vector<float> generate_noise(random_state& rng, vec2i dim)
{
    std::vector<float> ret;
    ret.resize(dim.x() * dim.y());

    for(auto& i : ret)
    {
        i = rand_det_s(rng.rng, 0, 1);
    }

    return ret;
}

float positive_fmod(float x, float y)
{
    float result = std::remainder(std::fabs(x), (y = std::fabs(y)));

    if (std::signbit(result))
    {
        result += y;
    }

    return result;
}

float simple_sample(const std::vector<float>& data, vec2f pos, vec2i dim)
{
    vec2f tl = floor(pos);
    vec2f br = ceil(pos);

    float xfrac = (pos.x() - tl.x());
    float yfrac = (pos.y() - tl.y());

    tl.x() = positive_fmod(tl.x(), dim.x() - 1);
    tl.y() = positive_fmod(tl.y(), dim.y() - 1);

    br.x() = positive_fmod(br.x(), dim.x() - 1);
    br.y() = positive_fmod(br.y(), dim.y() - 1);

    float tl_val = data[((int)tl.y()) * dim.x() + (int)tl.x()];
    float tr_val = data[((int)tl.y()) * dim.x() + (int)br.x()];
    float bl_val = data[((int)br.y()) * dim.x() + (int)tl.x()];
    float br_val = data[((int)br.y()) * dim.x() + (int)br.x()];

    float y1 = mix(tl_val, tr_val, xfrac);
    float y2 = mix(bl_val, br_val, xfrac);

    return mix(y1, y2, yfrac);
}

struct noise_data
{
    std::vector<float> noise_1;
    std::vector<float> noise_2;
    std::vector<float> noise_3;
    std::vector<float> noise_4;
    vec2i dim;

    noise_data(random_state& rng, vec2i _dim) : dim(_dim)
    {
        noise_1 = generate_noise(rng, dim);
        noise_2 = generate_noise(rng, dim);
        noise_3 = generate_noise(rng, dim);
        noise_4 = generate_noise(rng, dim);
    }

    float sample(vec2f pos)
    {
        float sample_freq = 0.005;

        vec2f warp = {simple_sample(noise_2, pos * sample_freq, dim), simple_sample(noise_3, pos * sample_freq, dim)};
        vec2f warp2 = vec2f{simple_sample(noise_2, pos * sample_freq * 10, dim), simple_sample(noise_3, pos * sample_freq * 10, dim)};

        vec2f wpos = pos + warp * 40 + warp2 * 20;

        float density = 0;

        density += simple_sample(noise_1, wpos, dim);
        density += simple_sample(noise_1, wpos / 2.f, dim) * 2;
        density += simple_sample(noise_1, wpos / 4.f, dim) * 4;
        density += simple_sample(noise_1, wpos / 8.f, dim) * 8;

        float width = dim.x();

        float water_width = width * 0.8;
        float land_width = width - water_width;

        vec2f centre = vec2f{dim.x(), dim.y()}/2.f;

        float distance_from_centre = (pos - centre).length();

        distance_from_centre = clamp(distance_from_centre, 0.f, width/2);

        float final_density = density / (8 + 4 + 2 + 1);

        if(distance_from_centre >= land_width/2)
        {
            float water_frac = (distance_from_centre - (land_width/2)) / (water_width/2);

            float subtractive_density = simple_sample(noise_4, wpos/32.f, dim);

            float low_val = final_density - subtractive_density;

            low_val = clamp(low_val, 0.f, 1.f);

            //return low_val;

            return mix(final_density, low_val, water_frac);

            //return mix(final_density, 0.f, (distance_from_centre - land_width/2) / (water_width/2));
        }

        return final_density;
    }
};

entt::entity create_tile_from_density(entt::registry& registry, random_state& rng, noise_data& noise, vec2i pos, vec2i dim)
{
    vec2f fpos = {pos.x(), pos.y()};
    fpos = fpos / vec2f{dim.x(), dim.y()};

    float fraction = noise.sample(fpos * 100);

    sprite_handle han;
    collidable coll;

    vec4f beach = srgb_to_lin_approx(vec4f{255, 218, 180, 255}/255.f);
    vec4f grass_col = srgb_to_lin_approx(vec4f{56, 217, 115, 255} / 255.f);

    float water_level = 0.2;
    float beach_to_water = 0.25;
    float grass_to_beach = 0.3;

    if(fraction < water_level)
    {
        han = get_sprite_handle_of(rng, tiles::WATER);

        float mfrac = fraction / water_level;

        mfrac = (mfrac + 1) / 2;

        han.base_colour.w() *= mfrac;

        coll.cost = -1;
    }
    /*else
    {
        han = get_sprite_handle_of(rng, tiles::BASE);

        float mfrac = (fraction - 0.2) / 0.8;

        float mov = 0.5;

        mfrac = (mfrac + mov) / (1 + mov);

        han.base_colour.w() *= mfrac;
    }*/

    else if(fraction < beach_to_water)
    {
        vec4f water = srgb_to_lin_approx(vec4f{60, 172, 215, 255}/255.f);

        float ffrac = (fraction - water_level) / (beach_to_water - water_level);

        han = get_sprite_handle_of(rng, tiles::BASE);
        han.base_colour = beach;

        han.base_colour = mix(water, beach, ffrac);

        coll.cost = 3;

        //han.base_colour.w() *= ffrac;
    }
    else if(fraction < grass_to_beach)
    {
        float ffrac = (fraction - beach_to_water) / (grass_to_beach - beach_to_water);

        han = get_sprite_handle_of(rng, tiles::BASE);

        vec4f gcol = grass_col;
        gcol.w() *= fraction;

        han.base_colour = mix(beach, gcol, ffrac);

        coll.cost = 2;
    }
    else
    {
        han = get_sprite_handle_of(rng, tiles::BASE);

        float ffrac = (fraction - grass_to_beach) / (1 - grass_to_beach);
        float mov = 0.25;
        ffrac = (ffrac + mov) / (1 + mov);

        han.base_colour.w() *= (fraction + mov) / (mov + 1);

        coll.cost = 1;
    }

    /*else
    {
        han = get_sprite_handle_of(rng, tiles::BASE);

        han.base_colour = srgb_to_lin_approx(vec4f{122, 68, 74, 255} / 255.f);
    }*/

    render_descriptor desc;
    desc.pos = vec2f{pos.x(), pos.y()} * TILE_PIX + vec2f{TILE_PIX / 2, TILE_PIX / 2};

    entt::entity base = registry.create();

    registry.assign<sprite_handle>(base, han);
    registry.assign<render_descriptor>(base, desc);
    registry.assign<overworld_tag>(base, overworld_tag());
    registry.assign<collidable>(base, coll);

    return base;
}

bool is_valid_castle_spawn(entt::registry& registry, tilemap& tmap, vec2f fpos)
{
    fpos = round(fpos);

    vec2i ipos = {fpos.x(), fpos.y()};

    if(ipos.x() < 0 || ipos.y() < 0 || ipos.x() >= tmap.dim.x() || ipos.y() >= tmap.dim.y())
        return false;

    for(entt::entity en : tmap.all_entities[ipos.y() * tmap.dim.x() + ipos.x()])
    {
        if(!registry.has<collidable>(en))
            continue;

        collidable& coll = registry.get<collidable>(en);

        if(!(coll.cost == 1))
            return false;
    }

    return a_star(registry, tmap, tmap.dim/2, vec2i{fpos.x(), fpos.y()}).has_value();

    //return true;
}

template<typename T>
std::optional<vec2i> square_search(entt::registry& registry, tilemap& tmap, vec2i start_pos, int max_distance, const T& is_valid)
{
    int bound = 0;
    int inner_bound = 0;

    while(bound <= max_distance)
    {
        for(int y=-bound; y<=bound; y++)
        {
            for(int x=-bound; x <= bound; x++)
            {
                if(std::max(abs(x), abs(y)) < inner_bound)
                    continue;

                if(is_valid(registry, tmap, vec2f{x + start_pos.x(), y + start_pos.y()}))
                    return vec2i{x, y} + start_pos;
            }
        }

        bound++;
        inner_bound++;
    }

    return std::nullopt;
}

entt::entity create_overworld(entt::registry& registry, random_state& rng, vec2i dim)
{
    entt::entity res = registry.create();

    tilemap tmap;
    tmap.create(dim);

    noise_data noise(rng, {100, 100});

    for (int y = 0; y < dim.y(); y++)
    {
        for (int x = 0; x < dim.x(); x++)
        {
            auto base = create_tile_from_density(registry, rng, noise, {x, y}, dim);

            tmap.add(base, {x, y});
        }
    }

    int factions = 5;

    float faction_radius = (dim.x() * 0.6) * 5.f / factions;

    float rwidth = dim.x() * 0.6/2;

    std::vector<vec2f> current_pos;

    current_pos.push_back({dim.x()/2.f, dim.y()/2.f});

    for(int i=0; i < factions - 1; i++)
    {
        float rangle = ((float)i / factions) * 2 * M_PI;

        vec2f pos = vec2f{1, 0}.rot(rangle) + vec2f{dim.x(), dim.y()}/2.f;

        current_pos.push_back(pos);
    }


    int iterations = 10;

    for(int i=0; i < iterations; i++)
    {
        for(int fid = 0; fid < (int)current_pos.size(); fid++)
        {
            if(fid == 0)
                continue;

            vec2f force = {0,0};
            vec2f my_pos = current_pos[fid];

            for(int oid = 0; oid < (int)current_pos.size(); oid++)
            {
                if(oid == fid)
                    continue;

                vec2f their_pos = current_pos[oid];

                vec2f diff = my_pos - their_pos;

                float len = diff.length();

                if(len > faction_radius)
                    continue;

                float move_frac = (faction_radius - len);

                force += (move_frac * diff).norm() * 5.f * (dim.x() / 64.f) / (float)iterations;
            }

            if(!is_valid_castle_spawn(registry, tmap, current_pos[fid] + force))
            {
                vec2i ipos = {current_pos[fid].x() + force.x(), current_pos[fid].y() + force.y()};

                auto compromise = square_search(registry, tmap, ipos, 2, is_valid_castle_spawn);

                if(!compromise.has_value())
                    continue;

                current_pos[fid] = {compromise.value().x(), compromise.value().y()};
                continue;
            }

            current_pos[fid] += force;

            current_pos[fid] = clamp(current_pos[fid], vec2f{0,0}, vec2f{dim.x()-1, dim.y()-1});
        }
    }

    //for(auto& i : current_pos)

    for(int idx = 0; idx < (int)current_pos.size(); idx++)
    {
        vec2f ipos = current_pos[idx];

        vec2f rounded = round(ipos);

        vec2i integer = {rounded.x(), rounded.y()};

        world_transform trans;
        trans.position = rounded * TILE_PIX + vec2f{TILE_PIX/2, TILE_PIX/2};

        sprite_handle handle = get_sprite_handle_of(rng, tiles::CASTLE_1);

        handle.base_colour *= team::colours.at(idx);

        entt::entity en = create_overworld_building(registry, handle, trans);

        tmap.add(en, integer);

        printf("End %i %i\n", integer.x(), integer.y());
    }

    registry.assign<tilemap>(res, tmap);
    registry.assign<overworld_tag>(res, overworld_tag());

    return res;
}

entt::entity create_dummy_army_at(entt::registry& registry, random_state& rng, vec2i pos, int team_id)
{
    world_transform transform;
    transform.position = vec2f{pos.x(), pos.y()} * TILE_PIX + vec2f{TILE_PIX/2, TILE_PIX/2};

    team base_team;
    base_team.type = team::NUMERIC;
    base_team.t = team_id;

    sprite_handle handle = get_sprite_handle_of(rng, tiles::SOLDIER_SPEAR);

    handle.base_colour *= team::colours.at(team_id);

    entt::entity army = create_unit_group(registry, base_team, handle, transform);

    int unit_count = 10;

    for(int i=0; i < unit_count; i++)
    {
        world_transform trans;
        trans.position = vec2f{pos.x(), pos.y()} * TILE_PIX + vec2f{TILE_PIX/2, TILE_PIX/2};

        damageable damage;

        entt::entity en = create_basic_unit(registry, base_team, get_sprite_handle_of(rng, tiles::SOLDIER_SPEAR), trans, damage);

        unit_group& ugroup = registry.get<unit_group>(army);

        ugroup.entities.push_back(en);
    }

    return army;
}

void debug_overworld(entt::registry& registry, entt::entity en, random_state& rng)
{
    tilemap& tmap = registry.get<tilemap>(en);

    vec2i half = tmap.dim/2;

    entt::entity army1 = create_dummy_army_at(registry, rng, {half.x(), half.y() - 1}, 0);
    entt::entity army2 = create_dummy_army_at(registry, rng, {half.x()+1, half.y() - 1}, 1);

    tmap.add(army1, {half.x(), half.y() - 1});
    tmap.add(army2, {half.x()+1, half.y() - 1});
}

entt::entity start_battle(entt::registry& registry, const std::vector<entt::entity>& armies)
{
    entt::entity ret = registry.create();

    return ret;
}
