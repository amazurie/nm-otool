/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 11:07:32 by amazurie          #+#    #+#             */
/*   Updated: 2019/03/01 14:37:01 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static int	print_usage(char *arg)
{
	ft_putstr("ft_nm: Unknown command line argument '");
	ft_putstr(arg);
	ft_putstr("'.\n");
	ft_putstr("usage: ft_nm [-juU] [file ...]\n");
	return (1);
}

static void	set_data(t_data *d)
{
	d->isot = 0;
	d->j = 0;
	d->u = 0;
	d->up_u = 0;
}

static char	check_symbol(t_data *d, char c)
{
	if (c == 'j')
		d->j = 1;
	else if (c == 'u')
		d->u = 1;
	else if (c == 'U')
		d->up_u = 1;
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
