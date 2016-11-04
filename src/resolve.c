#include "fillit.h"

static inline t_bool		set(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	int						new_offset;

	new_offset = t->offset.y + t->offset.x;
	value = (t->new_value >> new_offset);
	if (value & map->grid[0])
		return (FALSE);
	map->grid[0] |= value;
	t->new_offset[0] = new_offset;	
	return (TRUE);
}

static inline void			unset(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	int						new_offset;

	new_offset = t->offset.y + t->offset.x;
	value = (t->new_value >> new_offset);
	map->grid[0] ^= value;
}

t_bool						resolve(t_map *map, uint8_t tetri_index, uint8_t const size)
{
	t_tetrimino				*t;
	t_pos					dyn_pos_backup;

	t = &map->t[tetri_index];
	t->offset = map->dyn_pos[t->pattern_index];
	dyn_pos_backup = map->dyn_pos[t->pattern_index];
	t->offset.x += (t->offset.x > 0) ? t->gap_x : 0;
	while (t->offset.y <= t->max_offset)
	{
		while (t->offset.x <= t-> limit_line)
		{
			if (set(map, t))
			{
				map->dyn_pos[t->pattern_index] = t->offset;
				if (((tetri_index + 1 >= map->t_count)
					|| (is_enough_space(map)
						&& resolve(map, tetri_index + 1, size))))
					return (1);
				unset(map, t);
			}
			t->offset.x++;
		}
		t->offset.y += size;
		t->offset.x = 0;
	}
	map->dyn_pos[t->pattern_index] = dyn_pos_backup;
	return (0);
}
