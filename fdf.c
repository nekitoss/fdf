#include "h_fdf.h"

typedef struct	s_fdf
{
	int		fd;
	double	**arr;
	char	**text;
}				t_fdf;

int		main(int argc, char **argv)
{
	t_fdf	*ls;
	int		i;
	char	*line;
	i = 0;
	ls = (t_fdf *)ft_memalloc(sizeof(t_fdf));
	if (argc == 2)
	{
		ls->fd = open(argv[1], O_RDONLY);
		if (ls->fd >= 0)
		{
			ls->text = (char **)ft_newarr(0);
			while (get_next_line(ls->fd, &line) > 0)
			{
				ls->text = (char **)ft_extarr((void **)(ls->text), i + 1);
				ls->text[i] = line;
				i++;
			}
			printf("ft_cntstr=%d\n", ft_cntchr(line, ' ') + 1);
			// printf("%s\n", line);
		}
		else
			perror("Error opening the file");
	}
	else
		ft_putendl("Usage : ./fdf <filename>");
	return (0);
}