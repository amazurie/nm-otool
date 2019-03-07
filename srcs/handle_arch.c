/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 13:02:37 by amazurie          #+#    #+#             */
/*   Updated: 2019/03/06 11:28:26 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static int	handle_ar(t_data *d, char *name, struct ar_hdr *h, uint32_t magi)
{
	ft_putchar('\n');
	ft_putstr(d->arg);
	ft_putchar('(');
	ft_putstr(name);
	ft_putchar(')');
	ft_putstr(":\n");
	if (mapped_err(d, (char *)h + sizeof(struct ar_hdr)
				+ ft_atoi(h->ar_name + ft_strlen(AR_EFMT1))))
		return (1);
	d->map = (char *)h + sizeof(*h) + ft_atoi(h->ar_name + ft_strlen(AR_EFMT1));
	magi = *(uint32_t *)d->map;
	d->rev = magi == MH_CIGAM || magi == MH_CIGAM_64 || magi == FAT_CIGAM;
	if (magi == MH_CIGAM || magi == MH_MAGIC)
		handle_32b(d, d->map);
	else if (magi == MH_CIGAM_64 || magi == MH_MAGIC_64)
		handle_64b(d, d->map);
	else if (magi == FAT_MAGIC || magi == FAT_CIGAM)
		handle_fat(d, d->map);
	else
		ft_putendl("[!] archive error");
	return (0);
}

static int	handle_arch2(t_data *d, char **ptr,
		struct ar_hdr **header, uint32_t magi)
{
	char			*str;
	char			*name;
	size_t			len;

	if (mapped_err(d, *ptr))
		return (1);
	*header = (struct ar_hdr *)*ptr;
	if (ft_atoi((*header)->ar_size) <= 0)
		return (1);
	str = *ptr + sizeof(struct ar_hdr);
	if (ft_strncmp((*header)->ar_name, "#1/", 3) == 0)
		name = ft_strdup(ft_strstr((*header)->ar_name, ARFMAG)
				+ ft_strlen(ARFMAG));
	else
		name = ft_strndup((*header)->ar_name,
				ft_strlen_chr((*header)->ar_name, ' ') > 16 ? 16
				: ft_strlen_chr((*header)->ar_name, ' '));
	len = ft_strlen(str);
	while (!str[len++])
		;
	handle_ar(d, name, *header, magi);
	free(name);
	*ptr += ft_atoi((*header)->ar_size) + sizeof(struct ar_hdr);
	return (0);
}

int			handle_arch(t_data *d, char *ptr, uint32_t magi)
{
	struct ar_hdr	*header;

	if (d->isot)
	{
		ft_putstr("Archive : ");
		ft_putstr(d->arg);
	}
	ptr += SARMAG;
	header = (struct ar_hdr *)ptr;
	if (ft_strstr(header->ar_name, SYMDEF)
			|| ft_strstr(header->ar_name, SYMDEF_SORTED))
		ptr += ft_atoi(header->ar_size) + sizeof(struct ar_hdr);
	while (ptr && !handle_arch2(d, &ptr, &header, magi))
		;
	return (0);
}
