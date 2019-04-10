/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 10:23:12 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/10 13:51:31 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

void			puthexa(size_t d)
{
	size_t	i;

	if (d)
	{
		i = d / 16;
		puthexa(i);
		if (d % 16 < 10)
			ft_putchar('0' + d % 16);
		else
			ft_putchar('a' + (d % 16) - 10);
	}
	return ;
}

static long		sizehex(size_t n)
{
	long	i;

	i = 0;
	while (n > 0)
	{
		n /= 16;
		i++;
	}
	return (i);
}

static void		show_offset(t_data *d, t_syml *syml)
{
	size_t	i;
	int		len;
	char	c;

	len = (d->is32b) ? 8 : 16;
	c = (syml->type == 'U') ? ' ' : '0';
	i = len - sizehex(syml->addr);
	while (i--)
		ft_putchar(c);
	if (syml->addr > 0 || (syml->addr == 0 && c == '0'))
		puthexa(syml->addr);
	ft_putchar(' ');
}

void			show_res(t_data *d)
{
	t_syml	*syml;

	if (!d->symls)
		return ;
	syml = d->symls;
	while (syml)
	{
		if (syml->type && (!d->u || syml->type == 'U')
				&& (!d->up_u || syml->type != 'U')
				&& (!d->g || (syml->type > 64 && syml->type < 91)))
		{
			if (!d->u && !d->j)
			{
				show_offset(d, syml);
				ft_putchar(syml->type);
				ft_putchar(' ');
			}
			ft_putstr(syml->name);
			ft_putchar('\n');
		}
		syml = syml->next;
	}
}
