#include "h_fdf.h"

#define FLOAT_T double

typedef struct	s_fdf
{
	int		fd;
	FLOAT_T	**arr;
	char	**text;
	char	*line;
	int		ret;
	size_t	num_rows;
	size_t	num_cols;
	char	*addr;
	void	*mlx_ptr;
	size_t	size_x;
	size_t	size_y;
	void	*win_ptr;
	int		zoom;
	size_t	x1;
	size_t	y1;
	size_t	x2;
	size_t	y2;
	
}				t_fdf;

void		error(char *str)
{
	perror(str);
	exit(1);
}

void		error_msg(char *str)
{
	ft_putendl(str);
	exit(1);
}

void		print_struct(t_fdf *ls)
{
	size_t len;
	size_t i;
	size_t j;

	i = 0;
	len = 0;
	printf("Struct ----------------------\n");
	printf("fd = %d\n", ls->fd);
	printf("cols = %zu\n", ls->num_cols);
	printf("rows = %zu\n", ls->num_rows);
	printf("*arr=\n");
	len = ft_arrlen((void **)ls->arr);
	while (i < len)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			printf(" %4.1f", (ls->arr)[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("**text=\n");
	len = ft_arrlen((void **)ls->text);
	i = 0;
	while (i < len)
	{
		printf("%p:%s\n", (ls->text)[i],(ls->text)[i]);
		i++;
	}
	printf("-----------------------------\n");
}

void		del_struct(t_fdf *ls)
{
	int len;

	len = 0;
	len = ft_arrlen((void **)ls->arr);
	while (len >= 0)
	{
		ft_strdel((char **)&((ls->arr)[len]));
		len--;
	}
	len = ft_arrlen((void **)ls->text);
	while (len >= 0)
	{
		ft_strdel((char **)&((ls->text)[len]));
		len--;
	}
	// ft_arrdel((void ***)(&(ls->arr)));
	// ft_arrdel((void ***)(&(ls->text)));
}

void		draw_line(t_fdf *ls)
{
	int			error;
	int			error2;
	int			delta_y;
	int			delta_x;

	// printf("drw=%zu:%zu; %zu:%zu\n", ls->x1, ls->y1, ls->x2, ls->y2);
	delta_y = ABS(ls->y2 - ls->y1);
	delta_x = ABS(ls->x2 - ls->x1);
	error = delta_x - delta_y;
	error2 = error * 2;
	while (ls->x1 != ls->x2 || ls->y1 != ls->y2)
	{
		mlx_pixel_put(ls->mlx_ptr, ls->win_ptr, ls->x1 + 10,
			ls->y1 + 10, 0x000000FF);
		if (error2 > -delta_y)
		{
			error -= delta_y;
			ls->x1 += (ls->x1 < ls->x2 ? 1 : -1);
		}
		if (error2 < delta_x)
		{
			error += delta_x;
			ls->y1 += (ls->y1 < ls->y2 ? 1 : -1);
		}
	}
}

/*void		coun_coord(t_fdf *ls, size_t i, size_t j)
{
	ls->y1 = i * ls->zoom ;
	ls->x1 = j * ls->zoom ;
	ls->x2 = (j + ((j < ls->num_cols - 1) ? 1 : 0)) * ls->zoom ;
	ls->y2 = (i) * ls->zoom ;			
	draw_line(ls);
	ls->y1 = i * ls->zoom ;
	ls->x1 = j * ls->zoom ;
	ls->x2 = (j) * ls->zoom ;
	ls->y2 = (i + ((i < ls->num_rows - 1) ? 1 : 0)) * ls->zoom ;
	draw_line(ls);
}*/

void		print_lines(t_fdf *ls)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < ls->num_rows && i < ls->size_x)
	{
		j = 0;
		while (j < ls->num_cols && j < ls->size_y)
		{
			// if (ls->arr[i][j])
			// {
			// 	coun_coord(ls, i ,j);
			// }
			ls->y1 = i * ls->zoom ;
			ls->x1 = j * ls->zoom ;
			ls->x2 = (j + ((j < ls->num_cols - 1) ? 1 : 0)) * ls->zoom ;
			ls->y2 = (i) * ls->zoom ;			
			draw_line(ls);
			ls->y1 = i * ls->zoom ;
			ls->x1 = j * ls->zoom ;
			ls->x2 = (j) * ls->zoom ;
			ls->y2 = (i + ((i < ls->num_rows - 1) ? 1 : 0)) * ls->zoom ;
			draw_line(ls);
			j++;
		}
		i++;
	}
}

void		convert_row(t_fdf *ls, char **row, size_t r_num)
{
	size_t	i;

	i = 0;
	if (ls->num_cols &&
		ls->num_cols == ft_arrlen((void **)row))
	{
		ls->arr[r_num] = (FLOAT_T *)ft_memalloc(sizeof(FLOAT_T) * ls->num_cols);
		while (i < ls->num_cols)
		{
			(ls->arr)[r_num][i] = (FLOAT_T)ft_atoi(row[i]);
			i++;
		}
		// ft_strdel(&((ls->text)[i]));
	}
	else
		error_msg("Wrong line length in map!");
}

void		rows_into_array(t_fdf *ls)
{
	size_t	i;
	char	**tmp;

	i = 0;
	tmp = ft_strsplit((ls->text)[i], ' ');
	ls->num_cols = ft_arrlen((void **)tmp);
	ft_arrdel((void ***)&tmp);
	ls->arr = (FLOAT_T **)ft_newarr(ls->num_rows);
	while (i < ls->num_rows)
	{
		convert_row(ls, ft_strsplit((ls->text)[i], ' '), i);
		i++;
	}
}

void		count_save_rows(t_fdf *ls)
{
	size_t	i;

	ls->fd = open(ls->addr, O_RDONLY);
	while ((ls->ret = get_next_line(ls->fd, &(ls->line))) > 0)
	{
		ft_strdel(&(ls->line));
		(ls->num_rows)++;
	}
	if (ls->fd < 0 || ls->ret < 0)
		error("Error opening the file ");
	if (ls->num_rows == 0)
		error_msg("Error : Empty map file!");
	close(ls->fd);
	i = 0;
	ls->fd = open(ls->addr, O_RDONLY);
	ls->text = (char **)ft_newarr(ls->num_rows);
	while ((ls->ret = get_next_line(ls->fd, &(ls->line))) > 0)
	{
		(ls->text)[i] = ls->line;
		i++;
	}
	close(ls->fd);
}

void		print_in_window(t_fdf *ls)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < ls->num_rows && i < ls->size_x)
	{
		j = 0;
		while (j < ls->num_cols && j < ls->size_y)
		{
			if (ls->arr[i][j])
			{
				mlx_pixel_put(ls->mlx_ptr, ls->win_ptr, j * ls->zoom + 10,
					i * ls->zoom + 10, 0x0000FFFF);
				// printf("put pixel: x=%zu y=%zu\n", i, j);
			}
			else
				mlx_pixel_put(ls->mlx_ptr, ls->win_ptr,
					j * ls->zoom + 10, i * ls->zoom + 10, 0x00FF0000);
			j++;
		}
		i++;
	}
}

void		zoom_reset(t_fdf *ls)
{
	ls->zoom = 1;
	mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
	print_in_window(ls);
}

void		zoom_in(t_fdf *ls)
{
	ls->zoom += 5;
	mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
	print_in_window(ls);
}

void		zoom(t_fdf *ls, int step)
{
	if (step > 0)
		ls->zoom += step;
	else if (step < 0 && ls->zoom > -step)
		ls->zoom += step;
	else
		ls->zoom = 1;
	mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
	print_in_window(ls);
	print_lines(ls);
}

int			key_f(int key, void *ls_void)
{
	t_fdf	*ls;

	ls = (t_fdf *)ls_void;
	// mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
		// printf("key=%d\n", key);
	if (key == 69 || key == 24)//zoom in
		zoom(ls, 5);
	if (key == 78 || key == 27)//zoom out
		zoom(ls, -5);
	if (key == 82)//zoom reset
		zoom(ls, 0);
	if (key == 53)//exit by escape
	{
		mlx_destroy_window(ls->mlx_ptr, ls->win_ptr);
		exit(0);
	}
	// mlx_string_put(ls->mlx_ptr, ls->win_ptr, 150, 150, 0x00FFFF, ft_itoa(key));
	return (0);
}

int			test(void *ls_void)
{
	printf("loop_key = %p\n", ls_void);
	return (0);
}

int			main(int argc, char **argv)
{
	t_fdf	*ls;

	ls = (t_fdf *)ft_memalloc(sizeof(t_fdf));
	if (argc == 2)
	{
		ls->addr = argv[1];
		count_save_rows(ls);
		// printf("rows = %zu\n", ls->num_rows);
		rows_into_array(ls);
		// print_struct(ls);
		ls->mlx_ptr = mlx_init();
		ls->zoom = 50;
		ls->size_x = MIN(2560, (ls->num_cols - 1) * (ls->zoom) + 20);
		ls->size_y = MIN(1315, (ls->num_rows - 1) * (ls->zoom) + 20);
		printf("window %zu x %zu\n", ls->size_x, ls->size_y);
		if (ls->mlx_ptr)
		{
			ls->win_ptr = mlx_new_window(ls->mlx_ptr, ls->size_x, ls->size_y,
				ls->addr);
			if (ls->win_ptr)
			{
				print_in_window(ls);
				print_lines(ls);
				mlx_key_hook(ls->win_ptr, key_f, (void *)ls);
				// mlx_expose_hook (ls->win_ptr, test, (void *)ls);
				mlx_loop(ls->mlx_ptr);
			}
		}
		else
			error("MLX error:");
	}
	else
		ft_putendl("Usage : ./fdf <filename>");
	del_struct(ls);
	ls = NULL;
	return (0);
}
