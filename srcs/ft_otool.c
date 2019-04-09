/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 11:07:32 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/09 13:04:47 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static void	set_data(t_data *d)
{
	d->ot = 0;
	d->od = 0;
}

static int	print_usage(char *arg, char c)
{
	if (!arg)
		ft_putstr("error: ft_otool: at least one file must be specified\n");
	else
	{
		ft_putstr("error: ft_otool: unknown char '");
		ft_putchar(c);
		ft_putstr("' in flag");
		ft_putstr(arg);
		ft_putchar('\n');
	}
	ft_putstr("usage: ft_otool [-dt] [file ...]\n");
	return (1);
}

static int	check_symbol(t_data *d, char c)
{
	if (c == 't')
		d->ot = 1;
	else if (c == 'd')
		d->od = 1;
	else
		return (1);
	return (0);
}

static int	parse_opt(t_data *d, char *arg)
{
	int	i;

	i = 0;
	while (arg[++i])
	{
		if (check_symbol(d, arg[i]))
			return (print_usage(arg, arg[i]));
	}
	return (0);
}

int			main(int argc, char **argv)
{
	t_data	d;
	int		err;

	err = 0;
	set_data(&d);
	d.isot = 1;
	argv++;
	if (*argv && *argv[0] != '-')
		d.ot = 1;
	while (*argv && *argv[0] == '-')
	{
		if (parse_opt(&d, *argv))
			return (EXIT_FAILURE);
		argv++;
		argc--;
	}
	if (argc < 2)
		print_usage(NULL, 0);
	while (--argc)
	{
		err = nm_otool(&d, *argv);
		if (err == 0 || argc > 1)
			ft_putchar('\n');
		argv++;
	}
	return (err ? EXIT_FAILURE : EXIT_SUCCESS);
}
