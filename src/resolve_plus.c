#include"fillit.h"

static inline t_bool		set_plus(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	uint64_t				value2;
	int						new_offset[2];
	int						grid;

	new_offset[0] = t->offset.y + t->offset.x;
	grid = new_offset[0] > t->max_grid[1];
	value = t->new_value;
	value >>= new_offset[0];
	if (value & map->grid[grid] && new_offset[0] < (64 << grid))
		return (FALSE);
	else if (new_offset[0] > t->max_grid[grid])
	{
		new_offset[1] = new_offset[0] - (63 << grid);
		if (new_offset[1] > 0)
			value2 = (t->new_value >> new_offset[1]);
		else
			value2 = (t->new_value << -new_offset[1]);
		if (value2 & map->grid[grid + 1])
			return (FALSE);
		map->grid[grid + 1] |= value2;
	}
	map->grid[grid] |= value;
	t->new_offset[0] = new_offset[0];
	t->new_offset[1] = new_offset[1];
	return (TRUE);
}

static inline void			unset_plus(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	int						grid;


	grid = t->new_offset[0] > t->max_grid[1];
	value = t->new_value;
	value >>= t->new_offset[0];
	if (t->new_offset[0] < (64 << grid))
		map->grid[grid] ^= value;
	if (t->new_offset[0] > t->max_grid[grid])
	{
		if (t->new_offset[1] > 0)
			value = (t->new_value >> t->new_offset[1]);
		else
			value = (t->new_value << -t->new_offset[1]);
		map->grid[grid + 1] ^= value;
	}
}

t_bool		resolve_plus(t_map *map, uint8_t tetri_index, uint8_t const size)
{
	t_tetrimino		*t;
	t_pos			dyn_pos_backup;

	t = &map->t[tetri_index];
	t->offset = map->dyn_pos[t->pattern_index];
	dyn_pos_backup = map->dyn_pos[t->pattern_index];
	t->offset.x += (t->offset.x > 0) ? t->gap_x : 0;
	while (t->offset.y <= t->max_offset)
	{
		while (t->offset.x <= t->limit_line)
		{
			if (set_plus(map, t))
			{
				map->dyn_pos[t->pattern_index] = t->offset;
				if (((tetri_index + 1 >= map->t_count)
							|| resolve_plus(map, tetri_index + 1, size)))
					return (1);
				unset_plus(map, t);
			}
			t->offset.x++;
		}
		t->offset.y += size;
		t->offset.x = 0;
	}
	map->dyn_pos[t->pattern_index] = dyn_pos_backup;
	return (0);
}
