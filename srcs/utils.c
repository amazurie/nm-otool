/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 10:43:47 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/23 14:11:51 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

int				check(t_data *d, void *ptr)
{
	if (ptr > (void *)d->addr && ptr <= (void *)((char *)d->addr + d->size))
		return (1);
	return (0);
}

int				mapped_err(t_data *d, void *ptr)
{
	if (ptr < (void *)d->addr || ptr > (void *)((char *)d->addr + d->size))
		return (1);
	return (0);
}

uint64_t		rev_uint64_endian(uint64_t val, int rev)
{
	if (!rev)
		return (val);
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	val = (val << 16) | (val >> 16);
	return (val);
}

uint32_t		rev_uint32_endian(uint32_t val, int rev)
{
	if (!rev)
		return (val);
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	val = (val << 16) | (val >> 16);
	return (val);
}

int				put_err(char *msg)
{
	ft_putendl_fd(msg, 2);
	return (1);
}
