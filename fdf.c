#include "h_fdf.h"

typedef struct	s_fdf
{
	int		fd;
	double	**arr;
	char	**text;
	char	*line;
	int		ret;
	size_t	num_rows;
	size_t	num_cols;
	char	*addr;
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
	int len;
	int i;

	i = 0;
	len = 0;
	printf("Struct ----------------------\n");
	// printf("fd = %d\n", ls->fd);
	// printf("*arr=\n");
	// len = ft_arrlen(arr);
	// while (len-- > 0)
	// 	printf("%s\n", );
	printf("**text=\n");
	len = ft_arrlen((void **)ls->text);
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

void		count_save_rows(t_fdf *ls)
{
	size_t	i;

	ls->fd = open(ls->addr, O_RDONLY);
	while ((ls->ret = get_next_line(ls->fd, &(ls->line))) > 0)
	{
		ft_strdel(&(ls->line));
		(ls->num_rows)++;
	}
	if (ls->fd <0 || ls->ret < 0)
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
	print_struct(ls);
	close(ls->fd);
}

int			main(int argc, char **argv)
{
	t_fdf	*ls;

	ls = (t_fdf *)ft_memalloc(sizeof(t_fdf));
	if (argc == 2)
	{
		ls->addr = argv[1];
		count_save_rows(ls);
		printf("rows = %zu\n", ls->num_rows);
		// save_rows(ls);
	}
	else
		ft_putendl("Usage : ./fdf <filename>");
	del_struct(ls);
	ls = NULL;
	return (0);
}

































/*
void ft_test()
{
	char **str;
	char **arr;
	int	 i;
	const int len = 9;
	char *r="row_";
	char *tmp;
	int k;

	str = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (i < len)
	{
		tmp = ft_itoa(i);
		str[i] = ft_strjoin_d(&r, &tmp, 2);
		printf("str[%d]=%s\n", i, str[i]);
		i++;
	}

printf("\n\n\n");

	arr = (char **)ft_newarr(0);
	i = 0;
	while (i < len)
	{
		arr = (char **)ft_extarr((void ***)(&arr), i + 1);
		arr[i] = str[i];
		//printf("arr[%d]=%s\n", i, arr[i]);
		printf("straddr=%p | arr=%p\n", str[i], arr[i]);
		k = 0;
		while (k <= i)
		{
			printf("arr[%d]=%s\n", k, arr[k]);
			k++;
		}
		printf("\n");
		i++;
	}

printf("\n\n\n");

	i = 0;
	while (i < len)
	{
		if (arr[i] != str[i])
		{
			printf("straddr=%p | arr=%p\n", str[i], arr[i]);
			printf("str[%d]=%s | arr[%d]=%s\n", i, str[i], i, arr[i]);
		}
		i++;
	}
	// цвет 2/4/6/8
	// цвет не цвет
	// не букви
	// прямокутна
	// пуста

	// str = (char **)malloc(sizeof(char *) * 3);
	// str[0] = strdup("1row1");
	// str[1] = strdup("2row2");
	// str[2] = strdup("3row3");
	// printf("str addr=%p\n", str);
	// printf("str0: addr=%p data=%s\n", str[0],str[0]);
	// printf("str1: addr=%p data=%s\n", str[1],str[1]);
	// printf("str2: addr=%p data=%s\n", str[2],str[2]);

	// arr = (char **)ft_newarr(1);
	// printf("\n\n\narr_1 addr=%p\n", arr);
	// printf("arr0: addr=%p data=%s\n", arr[0],arr[0]);
	// printf("str0: addr=%p data=%s\n", str[0],str[0]);
	// arr[0] = str[0];
	// printf("\narr0: addr=%p data=%s\n", arr[0],arr[0]);
	// printf("str0: addr=%p data=%s\n", str[0],str[0]);


	// arr = (char **)ft_extarr((void **)(arr), 2);
	// printf("\n\n\narr_2 addr=%p\n", arr);
	// printf("arr0: addr=%p data=%s\n", arr[0],arr[0]);
	// printf("str0: addr=%p data=%s\n", str[0],str[0]);
	// printf("arr1: addr=%p data=%s\n", arr[1],arr[1]);
	// printf("str1: addr=%p data=%s\n", str[1],str[1]);
	// arr[1] = str[1];
	// printf("\narr0: addr=%p data=%s\n", arr[0],arr[0]);
	// printf("str0: addr=%p data=%s\n", str[0],str[0]);
	// printf("arr1: addr=%p data=%s\n", arr[1],arr[1]);
	// printf("str1: addr=%p data=%s\n", str[1],str[1]);

	// arr = (char **)ft_extarr((void **)(arr), 2);
	// printf("\n\n\narr_2 addr=%p\n", arr);
	// printf("arr0: addr=%p data=%s\n", arr[0],arr[0]);
	// printf("str0: addr=%p data=%s\n", str[0],str[0]);
	// printf("arr1: addr=%p data=%s\n", arr[1],arr[1]);
	// printf("str1: addr=%p data=%s\n", str[1],str[1]);
	// printf("arr1: addr=%p data=%s\n", arr[2],arr[2]);
	// printf("str1: addr=%p data=%s\n", str[2],str[2]);
	// arr[2] = str[2];
	// printf("\narr0: addr=%p data=%s\n", arr[0],arr[0]);
	// printf("str0: addr=%p data=%s\n", str[0],str[0]);
	// printf("arr1: addr=%p data=%s\n", arr[1],arr[1]);
	// printf("str1: addr=%p data=%s\n", str[1],str[1]);
	// printf("arr1: addr=%p data=%s\n", arr[2],arr[2]);
	// printf("str1: addr=%p data=%s\n", str[2],str[2]);
	// 	printf("arr3: addr=%p data=%s\n", arr[3],arr[3]);


}
*/
// int main(void)
// {
// 	ft_test();
// 	return 0;
// }
/*

int		main(int argc, char **argv)
{
	t_fdf	*ls;
	int		i;
	char	*line;

	
	while(1)
	{
	ls = (t_fdf *)ft_memalloc(sizeof(t_fdf));
	i = 0;
	if (argc == 2)
	{
		ls->fd = open(argv[1], O_RDONLY);
		if (ls->fd >= 0)
		{
			ls->text = (char **)ft_newarr(0);
			while (get_next_line(ls->fd, &line) > 0)
			{
				if (!(*line))
				{
					return (1);
				}
				printf("1: line=%p : %s\n", line, line);
				// print_struct(ls);
				ls->text = (char **)ft_extarr((void ***)&(ls->text), i + 1);
				(ls->text)[i] = line;
				printf("2: line=%p : %s\n", line, line);
				if (!(*line))
				{
					print_struct(ls);
					printf("RETURN ERROR\n");
					return (1);
				}
				// printf("lstx=%p : %s\n", (ls->text)[i], (ls->text)[i]);
				// print_struct(ls);
				i++;
			}
			// print_struct(ls);
			// printf("ft_cntstr=%d\n", ft_cntchr(line, ' ') + 1);
			// printf("%s\n", line);
		}
		else
			perror("Error opening the file");
	}
	else
		ft_putendl("Usage : ./fdf <filename>");
	close(ls->fd);
	del_struct(ls);
	ls = NULL;
	}
	return (0);
}*/

