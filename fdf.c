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
	{printf(" row = %zu\n", i);
		error_msg("Wrong line length in map!"); 
	}
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
		convert_row(ls, ft_strsplit((ls->text)[i],' '), i);
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
	while(i < ls->num_rows && i < ls->size_x)
	{
		j = 0;
		while(j < ls->num_cols && j < ls->size_y)
		{
			if (ls->arr[i][j])
			{
				mlx_pixel_put(ls->mlx_ptr,  ls->win_ptr, j*ls->zoom + 10, i*ls->zoom + 10, 0x0000FFFF);
				// printf("put pixel: x=%zu y=%zu\n", i, j);
			}
			else
				mlx_pixel_put(ls->mlx_ptr,  ls->win_ptr, j*ls->zoom + 10, i*ls->zoom + 10, 0x00FF0000);
			j++;
		}
		i++;
	}
}

void	zoom_reset(t_fdf *ls)
{
	ls->zoom = 1;
	mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
	print_in_window(ls);
}

void	zoom_in(t_fdf *ls)
{
	ls->zoom += 5;
	mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
	print_in_window(ls);
}

void	zoom_out(t_fdf *ls)
{
	if (ls->zoom > 5)
	{
		ls->zoom -= 5;
		mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
		print_in_window(ls);
	}
	else
		zoom_reset(ls);
}

int		key_f(int key, void *ls_void)
{
	t_fdf	*ls;

	ls = (t_fdf *)ls_void;
	// mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
		printf("key=%d\n", key);
	if (key == 69 || key == 24)
		zoom_in(ls);
	if (key == 78 || key == 27)
		zoom_out(ls);
	if (key == 82)
		zoom_reset(ls);
	if (key == 53)
	{
		mlx_destroy_window(ls->mlx_ptr, ls->win_ptr);
		exit(0);
	}
	// mlx_string_put(ls->mlx_ptr, ls->win_ptr, 150, 150, 0x00FFFF, ft_itoa(key));
	return (0);
}

int		test(void *ls_void)
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
		ls->size_x = MIN(2560 ,ls->num_cols * (ls->zoom) + 20);
		ls->size_y = MIN(1315 ,ls->num_rows * (ls->zoom) + 20);
		printf("window %zu x %zu\n", ls->size_x, ls->size_y);
		if (ls->mlx_ptr)
		{
			ls->win_ptr = mlx_new_window(ls->mlx_ptr, ls->size_x, ls->size_y,
				ls->addr);
			if (ls->win_ptr)
			{
				print_in_window(ls);
				mlx_key_hook (ls->win_ptr, key_f, (void *)ls);
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

// цвет 2/4/6/8
// цвет не цвет
// не букви