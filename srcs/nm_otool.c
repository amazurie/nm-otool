/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 11:07:32 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/24 12:51:21 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static int	load_arch(t_data *d)
{
	unsigned int	magi;

	if (mapped_err(d, d->map))
		return (put_err("The file was not recognized as a valid object file"));
	magi = *(unsigned int *)d->map;
	d->rev = (magi == MH_CIGAM || magi == MH_CIGAM_64 || magi == FAT_CIGAM);
	if ((d->isot && magi != FAT_MAGIC && magi != FAT_CIGAM
				&& ft_strncmp(d->map, ARMAG, SARMAG) != 0))
	{
		if (d->multi)
			ft_putchar('\n');
		ft_putstr(d->arg);
		ft_putstr(":\n");
	}
	if (magi == FAT_MAGIC || magi == FAT_CIGAM)
		return (handle_fat(d, d->map, d->rev));
	else if (magi == MH_MAGIC_64 || magi == MH_CIGAM_64)
		return (handle_64b(d, d->map));
	else if (magi == MH_MAGIC || magi == MH_CIGAM)
		return (handle_32b(d, d->map));
	else if (ft_strncmp(d->map, ARMAG, SARMAG) == 0)
		return (handle_arch(d, d->map, magi));
	else
		return (put_err("The file was not recognized as a valid object file"));
	return (0);
}

int			nm_otool(t_data *d, char *file)
{
	int			fd;
	struct stat	buf;
	int			err;

	err = 0;
	if ((fd = open(file, O_RDONLY)) < 0)
		return (put_err("error: file does not exist or permissions denied") + 1);
	if (fstat(fd, &buf) < 0)
		return (put_err("error: fstat failure"));
	if (!S_ISREG(buf.st_mode) && !S_ISLNK(buf.st_mode)
			&& !S_ISSOCK(buf.st_mode))
		return (put_err("error: not a binary file"));
	if ((d->addr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
		return (put_err("The file was not recognized as a valid object file"));
	d->map = d->addr;
	d->arg = file;
	d->size = buf.st_size;
	d->rmdup = 1;
	d->name = NULL;
	err = load_arch(d);
	if (munmap(d->addr, buf.st_size) < 0)
		return (put_err("error: munmap error"));
	if (close(fd) < 0)
		return (put_err("ft_nm: error: file closing error"));
	return (err);
}
