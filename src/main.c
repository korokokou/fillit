#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "fillit.h"

uint64_t	generate_dz_mask(int size)
{
	uint64_t	ret;
	uint64_t	mask;
	int			line;

	mask = 1;
	mask <<= 63;
	ret = 0;
	line = size;
	int temp = size;
	while (line > 0)
	{
		while (size)
		{
			if (size % 2 == 0)
				ret |= mask;
			mask >>= 1;
			size--;
		}
		if (line % 2 == 0)
			mask >>= 1;
		else
			mask <<= 1;
		size = temp;
		line--;
	}
	return ret;
}

void		solve(t_map *map)
{
	map->space_required = map->t_count * 4;
	map->size = ft_ceil_sqrt(map->space_required);
	while (map->size < 16)
	{
		map->total_space = map->size * map->size;
		clear(map);
		if (map->resolve(map, 0, map->size))
		{
			print_result_map(map);
			break ;
		}
		map->size++;
	}
}

int			main(int ac, char *av[])
{
	int		fd;
	t_map	map;

	if (ac != 2)
	{
		fillit_error_msg_exit("Usage: ./fillit [map.fillit]");
		return (-1);
	}
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		fillit_error_msg_exit("some open failed somewhere");
	ft_memset(&map, 0, sizeof(map));
	parse(fd, &map);
	close(fd);
	solve(&map);
	return (0);
}
