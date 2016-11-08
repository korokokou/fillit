#include "fillit.h"

inline static int	count_adj(t_map *map, int8_t offset, const uint8_t size)
{
	uint64_t	tmp_value;
	uint8_t		adj_sum;
	uint8_t		grid_index;

	if (offset > 0 || map->total_space < offset)
		return (0);
	grid_index = offset >> 6;
	tmp_value = 1;
	tmp_value <<= (offset & 63);
	print_dyn_piece(tmp_value, 8);
	if (tmp_value & map->dz[grid_index])
		return (0);
	print_dyn_piece(tmp_value & map->dz[grid_index], 8);
	print_dyn_piece(map->dz[grid_index], 8);
	exit(-1);
	map->dz[grid_index] |= tmp_value;
	adj_sum = 1;
	adj_sum += count_adj(map, offset - size, size);
	adj_sum += count_adj(map, offset - 1, size);
	adj_sum += count_adj(map, offset + size, size);
	adj_sum += count_adj(map, offset + 1, size);
	return (adj_sum);
}

int					count_trailing_zero(uint64_t v, int c)					
{
	if ((v & 0xffffff) == 0) 
	{  
		v >>= 32;  
		c += 32;
	}
	if ((v & 0xffff) == 0) 
	{  
		v >>= 16;  
		c += 16;
	}
	if ((v & 0xff) == 0) 
	{  
		v >>= 8;  
		c += 8;
	}
	if ((v & 0xf) == 0) 
	{  
		v >>= 4;
		c += 4;
	}
	if ((v & 0x3) == 0) 
	{  
		v >>= 2;
		c += 2;
	}
	c -= v & 0x1;
	return (c);
}
static int count = 1;
t_bool				is_enough_space(t_map *map)
{
	uint8_t			adj[3];
	uint8_t			n_dz;
	uint8_t			limit;
	int8_t			offset;
	int				grid;
	uint64_t		temp;

	n_dz = 0;
	offset = map->total_space;
	limit = offset - map->space_required;
	ft_memcpy(map->dz, map->grid, sizeof(map->grid));
	grid = offset >> 6;
	temp = map->dz[grid];
	offset = offset & 63;
	temp >>= 64 - offset; 
	adj[1] = 0;
	printf(" start %d %d %d\n", offset, adj[1], grid);
	print_dyn_piece(map->grid[0], map->size);
	while (grid >= 0)
	{	
		while (offset > 0)
		{
			if (temp & 1)
			{
			    adj[1] = __builtin_ctzl(~temp);
				adj[2] = adj[1];
			}
			else
			{
				adj[0] = __builtin_ctzl(temp);
				if (adj[0] < 4 && adj[1] > map->size)
				{		n_dz += adj[0];
	
					printf("n_dz %d\n", n_dz);			
				}
				if (n_dz > limit)
				{
					printf("out\n");
					return (FALSE);
				 }
				adj[2] = adj[0];
			}
			printf(" inside offset: %d zeros: %d ones: %d  total %d\n", offset, adj[0], adj[1], adj[2]);
			offset -= adj[2];
			temp >>= adj[2];
		}
		offset = 64;
		grid--;
	}
	count++;
		printf(" next  \n\n");
	if (count == 15)
	{
		exit(-1);
	}
	return (TRUE);
}
