/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 13:02:37 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/24 14:54:32 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static void	print_h(struct ar_hdr *h)
{
	int	i;

	ft_putchar('\n');
	i = 0;
	while (ft_isdigit(h->ar_mode[i]))
		i++;
	ft_putnchar('0', i - 1 < 8 ? 8 - i - 1 : 0);
	ft_putnstr(h->ar_mode, i);
	ft_putchar(' ');
	i = 0;
	while (ft_isdigit(h->ar_uid[i]))
		i++;
	ft_putnstr(h->ar_uid, i);
	ft_putchar('/');
	i = 0;
	while (ft_isdigit(h->ar_gid[i]))
		i++;
	ft_putnstr(h->ar_gid, i);
	ft_putchar(' ');
	i = 0;
	while (ft_isdigit(h->ar_size[i]))
		i++;
	ft_putnstr(h->ar_size, i);
	ft_putchar(' ');
	i = 0;
	while (ft_isdigit(h->ar_date[i]))
		i++;
	ft_putnstr(h->ar_date, i);
	ft_putchar(' ');
	ft_putnstr(ft_strstr(h->ar_name, "#1/"), 5);
}

static int	handle_ar(t_data *d, struct ar_hdr *h, uint32_t magi)
{
	if (mapped_err(d, (char *)h + sizeof(struct ar_hdr)
				+ ft_atoi(h->ar_name + ft_strlen(AR_EFMT1))))
		return (1);
	d->map = (char *)h + sizeof(*h) + ft_atoi(h->ar_name + ft_strlen(AR_EFMT1));
	magi = *(uint32_t *)d->map;
	d->rev = magi == MH_CIGAM || magi == MH_CIGAM_64 || magi == FAT_CIGAM;
	if (d->oa)
	{
		print_h(h);
		return 0;
	}
	if (magi != MH_CIGAM && magi != MH_MAGIC && magi != MH_CIGAM_64
		&& magi != MH_MAGIC_64 && magi != FAT_MAGIC && magi != FAT_CIGAM)
		return (0);
	if (!d->o)
	{
		ft_putchar('\n');
		ft_putstr(d->arg);
		ft_putchar('(');
		ft_putstr(d->name);
		ft_putchar(')');
		ft_putstr(":\n");
	}
	if (magi == MH_CIGAM || magi == MH_MAGIC)
		return (handle_32b(d, d->map));
	else if (magi == MH_CIGAM_64 || magi == MH_MAGIC_64)
		return (handle_64b(d, d->map));
	else if (magi == FAT_MAGIC || magi == FAT_CIGAM)
		return (handle_fat(d, d->map, d->rev));
	return (0);
}

static int	handle_arch2(t_data *d, char **ptr,
		struct ar_hdr **header, uint32_t magi)
{
	char			*str;
	size_t			len;

	if (mapped_err(d, *ptr))
		return (1);
	*header = (struct ar_hdr *)*ptr;
	if (ft_atoi((*header)->ar_size) <= 0)
		return (1);
	str = *ptr + sizeof(struct ar_hdr);
	if (ft_strncmp((*header)->ar_name, "#1/", 3) == 0)
		d->name = ft_strdup(ft_strstr((*header)->ar_name, ARFMAG)
				+ ft_strlen(ARFMAG));
	else
		d->name = ft_strndup((*header)->ar_name,
				ft_strlen_chr((*header)->ar_name, ' ') > 16 ? 16
				: ft_strlen_chr((*header)->ar_name, ' '));
	len = ft_strlen(str);
	while (!str[len++])
		;
	handle_ar(d, *header, magi);
	free(d->name);
	d->name = NULL;
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
	if (d->oa)
		print_h(header);
	while (ptr && !handle_arch2(d, &ptr, &header, magi))
		;
	return (0);
}
