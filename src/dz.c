#include "fillit.h"

int		get_bit_from_int(uint16_t value, int n)
{
	return (value & 0x8000 >> n);
}

int		count_adj(t_map *map, int x, int y, int adj)
{
	int	bit;

	if (x < 0 || x >= map->size || y < 0 || y >= map->size)
		return (0);
	bit = get_bit_from_int(map->mdz[y], x);	
	if (bit)
		return (adj);
	map->mdz[y] |= 0x8000 >> x;
	return (count_adj(map, x - 1, y, adj) + count_adj(map, x, y - 1, adj) + count_adj(map, x + 1, y, adj) + count_adj(map, x, y + 1, adj) + 1);
}

int		count_deadzones(t_map *map)
{
	int	dz;
	int	x;
	int	y;
	int	adj;

	dz = 0;
	//clone map
	ft_memcpy(map->mdz, map->m, sizeof(map->m));
	print_map(map->mdz, map->size);
	y = 0;
	while (y < map->size)
	{
		x = 0;
		while (x < map->size)
		{
			//check des cases adjacentes.
			adj = count_adj(map, x, y, 0);
			if (adj < 4)
				dz += adj;
			printf("[%d,%d] adj=%d dz=%d\n", y, x, adj, dz);
			x++;
		}
	y++;
	}	
	returne(dz);
}

t_bool		is_too_much_dz(t_map *map)
{
	map->n_dz = count_deadzones(map);
	if (map->total_space - map->n_dz <= map->space_required)
		return (FALSE);
	return (TRUE);
}