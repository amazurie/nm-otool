/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 11:07:32 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/23 13:47:06 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static int	print_usage(char *arg)
{
	ft_putstr_fd("ft_nm: Unknown command line argument '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("'.\n", 2);
	ft_putstr_fd("usage: ft_nm [-juU] [file ...]\n", 2);
	return (1);
}

static void	set_data(t_data *d)
{
	d->isot = 0;
	d->j = 0;
	d->u = 0;
	d->up_u = 0;
	d->g = 0;
	d->o = 0;
}

static char	check_symbol(t_data *d, char c)
{
	if (c == 'j')
		d->j = 1;
	else if (c == 'u')
		d->u = 1;
	else if (c == 'o')
		d->o = 1;
	else if (c == 'U')
		d->up_u = 1;
	else if (c == 'g')
		d->g = 1;
	else
		return (1);
	return (0);
}

static char	parse_opt(t_data *d, char *arg)
{
	int	i;

	i = 0;
	if (!arg || !*arg)
		return (1);
	while (arg[++i])
	{
		if (check_symbol(d, arg[i]))
			return (print_usage(arg));
	}
	return (0);
}

int			main(int argc, char **argv)
{
	t_data	d;
	int		err;

	err = 0;
	set_data(&d);
	argv++;
	while (*argv && *argv[0] == '-')
	{
		if (parse_opt(&d, *argv))
			return (EXIT_FAILURE);
		argv++;
		argc--;
	}
	if (argc < 2)
		nm_otool(&d, "a.out");
	d.multi = argc > 2;
	while (--argc)
	{
		err = nm_otool(&d, *argv);
		argv++;
	}
	return (err ? EXIT_FAILURE : EXIT_SUCCESS);
}
