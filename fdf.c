#include "h_fdf.h"

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

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
	int len;
	int i;
	int j;

	i = 0;
	len = 0;
	printf("Struct ----------------------\n");
	printf("fd = %d\n", ls->fd);
	printf("cols = %d\n", ls->num_cols);
	printf("rows = %d\n", ls->num_rows);
	printf("*orig=\n");
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			printf(" %4.1f:%4.1f", ((ls->orig)[i][j]).x, ((ls->orig)[i][j]).y);
			j++;
		}
		printf("\n");
		i++;
	}
	i = 0;
	printf("*arr=\n");
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			printf(" %4.1f:%4.1f", ((ls->arr)[i][j]).x, ((ls->arr)[i][j]).y);
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
	// len = ft_arrlen((void **)ls->arr);
	// while (len >= 0)
	// {
	// 	ft_strdel((char **)&((ls->arr)[len]));
	// 	len--;
	// }
	len = ft_arrlen((void **)ls->text);
	while (len >= 0)
	{
		ft_strdel((char **)&((ls->text)[len]));
		len--;
	}
	// ft_arrdel((void ***)(&(ls->arr)));
	// ft_arrdel((void ***)(&(ls->text)));
}

FLOAT_T		rad(double deg)
{
	return (deg * M_PI / 180.0);
}

void		find_height(t_fdf *ls)
{
	int	i;
	int	j;

	ls->max_h = (ls->arr[0][0]).z;
	ls->min_h = (ls->arr[0][0]).z;
	i = 0;
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			if (ls->max_h < (ls->arr[i][j]).z)
				ls->max_h = (ls->arr[i][j]).z;
			if (ls->min_h > (ls->arr[i][j]).z)
				ls->min_h = (ls->arr[i][j]).z;
			j++;
		}
		i++;
	}
	printf("max_h = %d; min_h = %d\n", ls->max_h, ls->min_h);
}

void			make_color(t_fdf *ls)
{
	int	i;
	int	j;
	int red;
	int blue;

	find_height(ls);
	i = 0;
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			blue = (255 / (ls->max_h - ls->min_h + 1)) * ((ls->orig)[i][j]).z;
			red = 255 - blue;
			((ls->orig)[i][j]).color = red;
			((ls->orig)[i][j]).color = ((ls->orig)[i][j]).color << 16;
			((ls->orig)[i][j]).color += blue;
			((ls->arr)[i][j]).color = ((ls->orig)[i][j]).color;
			j++;
		}
		i++;
	}
}

void		convert_row(t_fdf *ls, char **row, int r_num)
{
	int	i;

	i = 0;
	if (ls->num_cols &&
		ls->num_cols == (int)ft_arrlen((void **)row))
	{
		ls->orig[r_num] = (t_point *)ft_memalloc(sizeof(t_point) * ls->num_cols);
		ls->arr[r_num] = (t_point *)ft_memalloc(sizeof(t_point) * ls->num_cols);
		while (i < ls->num_cols)
		{
			((ls->orig)[r_num][i]).x = (FLOAT_T)i;
			((ls->orig)[r_num][i]).y = (FLOAT_T)r_num;
			((ls->orig)[r_num][i]).z = (FLOAT_T)ft_atoi(row[i]);
			i++;
		}
		// ft_strdel(&((ls->text)[i]));
		ft_memcpy((void *)ls->arr[r_num], (void *)ls->orig[r_num], sizeof(t_point) * ls->num_cols);
	}
	else
		error_msg("Wrong line length in map!");
}

void		rows_into_array(t_fdf *ls)
{
	int	i;
	char	**tmp;

	i = 0;
	tmp = ft_strsplit((ls->text)[i], ' ');
	ls->num_cols = ft_arrlen((void **)tmp);
	ft_arrdel((void ***)&tmp);
	ls->arr = (t_point **)ft_newarr(ls->num_rows);
	ls->orig = (t_point **)ft_newarr(ls->num_rows);
	while (i < ls->num_rows)
	{
		convert_row(ls, ft_strsplit((ls->text)[i], ' '), i);
		i++;
	}
}

void		count_save_rows(t_fdf *ls)
{
	int	i;

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

void		redraw(t_fdf *ls)
{
	// printf("clear ptr\n");
	mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
	// printf("destroy ptr=%p\n", ls->img_ptr);
	if (ls->img_ptr)
		mlx_destroy_image(ls->mlx_ptr, ls->img_ptr);
	// printf("new_img %dx%d\n", ls->size_x, ls->size_y);
	ls->img_ptr = mlx_new_image(ls->mlx_ptr, ls->size_x, ls->size_y);
	if (!(ls->img_ptr))
		error("IMG_not created");
	// printf("new_ptr=%p\n", ls->img_ptr);
	// printf("get_data\n");
	ls->data = mlx_get_data_addr(ls->img_ptr, &(ls->bits_per_pixel), &(ls->size_line), &(ls->endian));
	// printf("make_image\n");
	// make_image(ls);
	// printf("rotate\n");
	// if (ls->angle_z)
		rotate(ls);
	// else
		// make_image(ls);
	// printf("make_lines\n");
	// make_lines(ls);
	// printf("\n\n"); printBits (ls->size_line*ls->num_rows, ls->data);
	printf("put image\n");
	mlx_put_image_to_window(ls->mlx_ptr, ls->win_ptr, ls->img_ptr, 10, 10);
	// printf("\n"); printBits ((ls->size_x - 20)*(ls->size_y - 20), ls->data);
	// printBits (4*5, (ls->data)+(ls->size_line));
	// printf("\n");
	// print_in_window(ls);
	// print_lines(ls);
	// print_struct(ls);
}

int			key_f(int key, void *ls_void)
{
	t_fdf	*ls;

	ls = (t_fdf *)ls_void;
	// mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
		printf("key=%d\n", key);
	if (key == 69 || key == 24)//zoom in
		ls->zoom *= 1.5;
	if (key == 78 || key == 27)//zoom out
		ls->zoom *= 0.5;
	if (key == 82)//zoom reset
		ls->zoom = 1;
	if (key == 0)//A=left
		(ls->angle_x) -= 10;
	if (key == 2)//D=right
		(ls->angle_x) += 10;
	if (key == 1)//S=down
		(ls->angle_z) -= 10;
	if (key == 13)//W=UP
		(ls->angle_z) += 10;
	if (key == 12)//Q=clock
		(ls->angle_y) -= 10;
	if (key == 14)//E=clock
		(ls->angle_y) += 10;
	/*if (key == 123)//left
		(ls->angle_x) -= 10;
	if (key == 124)//right
		(ls->angle_x) += 10;
	if (key == 125)//down
		(ls->angle_z) -= 10;
	if (key == 126)//UP
		(ls->angle_z) += 10;*/
	// if (ABS(ls->angle_x) == 360)
	// 	ls->angle_x = 0;
	// if (ABS(ls->angle_y) == 360)
	// 	ls->angle_y = 0;
	// if (ABS(ls->angle_z) == 360)
	// 	ls->angle_z = 0;
	if (key == 15)
	{
		ls->angle_x = 0;
		ls->angle_y = 0;
		ls->angle_z = 0;
	}
	if (key == 53)//exit by escape
	{
		mlx_destroy_window(ls->mlx_ptr, ls->win_ptr);
		exit(0);
	}
	printf("zoom=%.0f\n", ls->zoom);
	redraw(ls);
	// mlx_string_put(ls->mlx_ptr, ls->win_ptr, 150, 150, 0x00FFFF, ft_itoa(key));
	return (0);
}

int			test(void *ls_void)
{
	printf("loop_key = %p\n", ls_void);
	return (0);
}

void		draw_line(t_fdf *ls)
{
	int			error;
	int			error2;
	int			delta_y;
	int			delta_x;

	// printf("drw=%d:%d; %d:%d\n", ls->x1, ls->y1, ls->x2, ls->y2);
	delta_y = ABS(ls->y2 - ls->y1);
	delta_x = ABS(ls->x2 - ls->x1);
	error = delta_x - delta_y;
	error2 = error * 2;
	while ((ls->x1 != ls->x2 || ls->y1 != ls->y2))
	{
		// mlx_pixel_put(ls->mlx_ptr, ls->win_ptr, ls->x1 + 10,
			// ls->y1 + 10, 0x000000FF);
		pixel_to_img(ls, ls->y1, ls->x1, 0x000000FF);
		// if ( ls->y1 == 1 && ls->x1 < 10)
			// printf("%d:%d ", ls->y1, ls->x1);
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
	// printf("\n");
}

void		pixel_to_img(t_fdf *ls, int i, int j, int color)
{
	int *tmp;

	i += ls->center_x;
	j += ls->center_y;
	tmp = (int *)&(ls->data[IMG_ROW(i) + IMG_COL(j)]);
	if ((i < ls->size_y) && (j <  ls->size_x) && i >= 0 && j >= 0)
		*tmp = color;
}

void		rotate(t_fdf *ls)
{
	int	i;
	int	j;
	FLOAT_T x;
	FLOAT_T y;
	FLOAT_T z;

	printf("rotate\nangle: x=%d; y=%d; z=%d\n", ls->angle_x, ls->angle_y, ls->angle_z);
	i = 0;
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			
			y = ((ls->orig[i][j]).x * cos(rad(ls->angle_y)) - (ls->orig[i][j]).y * sin(rad(ls->angle_y))) * ls->zoom;
			x = ((ls->orig[i][j]).x * sin(rad(ls->angle_y)) + (ls->orig[i][j]).y * cos(rad(ls->angle_y))) * ls->zoom;
			(ls->arr[i][j]).x = x;
			(ls->arr[i][j]).y = y;
			z = ((ls->orig[i][j]).z * ls->zoom * cos(rad(ls->angle_z)) - (ls->arr[i][j]).x * sin(rad(ls->angle_z)));
			x = ((ls->orig[i][j]).z * ls->zoom * sin(rad(ls->angle_z)) + (ls->arr[i][j]).x * cos(rad(ls->angle_z)));
			(ls->arr[i][j]).x = x;
			(ls->arr[i][j]).z = z;
			y = ((ls->arr[i][j]).y * cos(rad(ls->angle_x)) - (ls->arr[i][j]).z * sin(rad(ls->angle_x)));
			z = ((ls->arr[i][j]).y * sin(rad(ls->angle_x)) + (ls->arr[i][j]).z * cos(rad(ls->angle_x)));
			(ls->arr[i][j]).z = z;
			(ls->arr[i][j]).y = y;
			pixel_to_img(ls, (ls->arr[i][j]).x, (ls->arr[i][j]).y, (ls->arr[i][j]).color);
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(ls->mlx_ptr, ls->win_ptr, ls->img_ptr, 10, 10);
}

void		make_image(t_fdf *ls)
{
	int	i;
	int	j;
printf("make image\n");
	i = 0;
	while (i < ls->num_rows && (i * ls->zoom) < ls->size_y)
	{
		j = 0;
		while (j < ls->num_cols && (j * ls->zoom) < ls->size_x)
		{
			// printf("%d:%d ", i * ls->zoom, j * ls->zoom);
			(ls->arr[i][j]).x = (ls->orig[i][j]).x * ls->zoom;
			(ls->arr[i][j]).y = (ls->orig[i][j]).x * ls->zoom;
			// printf("arr[%d][%d]= %f:%f\n", i,j,(ls->arr[i][j]).x ,(ls->arr[i][j]).y);
			// (ls->arr[i][j]).z *= zoom;
			if ((ls->arr[i][j]).z)
				pixel_to_img(ls, (i * ls->zoom), (j * ls->zoom), 0x0000FFFF);
			else
				pixel_to_img(ls, (i * ls->zoom), (j * ls->zoom), 0x00FF0000);
			j++;
		}
		// printf("\n");
		i++;
	}
}

void		make_lines(t_fdf *ls)
{
	int	i;
	int	j;

	i = 0;
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
/*
			ls->y1 = i * ls->zoom ;
			ls->x1 = j * ls->zoom ;
			ls->x2 = (j + ((j < ls->num_cols - 1) ? 1 : 0)) * ls->zoom ;
			ls->y2 = (i) * ls->zoom ;
			// printf("%d:%d -> %d:%d\n", ls->x1, ls->y1, ls->x2, ls->y2);			
			draw_line(ls);
			ls->y1 = i * ls->zoom ;
			ls->x1 = j * ls->zoom ;
			ls->x2 = (j) * ls->zoom ;
			ls->y2 = (i + ((i < ls->num_rows - 1) ? 1 : 0)) * ls->zoom ;
*/
			// if ((ls->arr[i][j]).z > 1)
			{
			ls->y1 = (ls->arr[i][j]).y;
			ls->x1 = (ls->arr[i][j]).x;
			ls->x2 = (ls->arr[i][(j + ((j < ls->num_cols - 1) ? 1 : 0))]).x;
			ls->y2 = (ls->arr[i][j]).y;
			// printf("i=%d; j=%d\n", i, (j + ((j < ls->num_cols - 1) ? 1 : 0)));
			// printf("1)%d:%d -> %d:%d\n", ls->x1, ls->y1, ls->x2, ls->y2);			
			draw_line(ls);
			ls->y1 = (ls->arr[i][j]).y;
			ls->x1 = (ls->arr[i][j]).x;
			ls->x2 = (ls->arr[i][j]).x;
			ls->y2 = (ls->arr[(i + ((i < ls->num_rows - 1) ? 1 : 0))][j]).y;
			// printf("i=%d; j=%d\n",(i + ((i < ls->num_rows - 1) ? 1 : 0)),j );
			// printf("2)%d:%d -> %d:%d\n\n", ls->x1, ls->y1, ls->x2, ls->y2);
			draw_line(ls);
			j++;
			}
		}
		i++;
	}
}

int			main(int argc, char **argv)
{
	t_fdf	*ls;
	// t_img	*pixel;
	// int		*test;

	ls = (t_fdf *)ft_memalloc(sizeof(t_fdf));
	if (argc == 2)
	{
		ls->addr = argv[1];
		count_save_rows(ls);
		// printf("rows = %d\n", ls->num_rows);
		
		rows_into_array(ls);
		make_color(ls);
		// print_struct(ls);
		
		ls->mlx_ptr = mlx_init();
		ls->zoom = 50;
		ls->angle_z = -45;
		ls->angle_x = -30;
		ls->size_x = MIN(2560, (ls->num_cols - 1) * (ls->zoom) + 1);
		ls->size_y = MIN(1315, (ls->num_rows - 1) * (ls->zoom) + 1);
		ls->size_x = MIN(2560, (int)ceil(sqrt(ls->size_x * ls->size_x + ls->size_y * ls->size_y)));
		ls->size_y = MIN(1315, (int)ceil(sqrt(ls->size_x * ls->size_x + ls->size_y * ls->size_y)));
		ls->center_x = ls->size_x / 2;
		ls->center_y = ls->size_y / 2;
		printf("window %d x %d\n", ls->size_x, ls->size_y);
		if (ls->mlx_ptr)
		{
			ls->win_ptr = mlx_new_window(ls->mlx_ptr, ls->size_x + 19, ls->size_y + 19,
				ls->addr);
			if (ls->win_ptr)
			{

				printf("num_cols=%d, num_rows=%d\n", ls->num_cols, ls->num_rows);
				// printf("%p = %p\n", mlx_get_data_addr(ls->img_ptr, &(ls->bits_per_pixel), &(ls->size_line), &(ls->endian)), ls->data);
				redraw(ls);
				printf("bits pp=%d(bits)=%d(bytes), size_line=%d(bytes), endian=%d\n", ls->bits_per_pixel, ls->bits_per_pixel/8,ls->size_line, ls->endian);
				// make_image(ls);
				// make_lines(ls);
				// printBits (ls->size_line*ls->num_rows, ls->data);
				
				// mlx_put_image_to_window(ls->mlx_ptr, ls->win_ptr, ls->img_ptr, 10, 10);
				
				// mlx_do_key_autorepeaton(ls->mlx_ptr);
				// print_in_window(ls);
				// print_lines(ls);
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