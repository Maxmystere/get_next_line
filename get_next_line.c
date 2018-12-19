/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magrab <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 18:28:44 by magrab            #+#    #+#             */
/*   Updated: 2018/11/19 18:28:47 by magrab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Work
** Use only if \n inside tabfd
*/

static int		isinbuffer(const int fd, char **line, char *tabfd[MAX_FD])
{
	int		npos;
	char	*tmp;

	npos = ft_strsrchr(tabfd[fd], '\n');
	if (!(tmp = malloc((npos + 1) * sizeof(char))))
		return (-1);
	tmp[npos] = 0;
	ft_strncpy(tmp, tabfd[fd], npos);
	*line = tmp;
	tmp = ft_strdup(tabfd[fd] + npos + 1);
	if (tmp && *tmp == 0)
		ft_strdel(&tmp);
	if (tabfd[fd])
		ft_strdel(&tabfd[fd]);
	tabfd[fd] = tmp;
	return (1);
}

static int		gnl(const int fd, char **line, int xread, char *tabfd[MAX_FD])
{
	char		reader[BUFF_SIZE + 1];
	char		*save;

	reader[BUFF_SIZE] = 0;
	while (ft_strchr(tabfd[fd], '\n') == NULL && xread == BUFF_SIZE)
	{
		xread = read(fd, reader, BUFF_SIZE);
		save = ft_strnjoin(tabfd[fd], reader, xread);
		ft_strdel(&tabfd[fd]);
		tabfd[fd] = save;
	}
	if (ft_strchr(tabfd[fd], '\n'))
		return (isinbuffer(fd, line, tabfd));
	if (0 <= xread && tabfd[fd][0])
	{
		*line = tabfd[fd];
		tabfd[fd] = NULL;
		return (1);
	}
	return (-1);
}

int				get_next_line(const int fd, char **line)
{
	static char	*tabfd[MAX_FD];
	int			xread;

	if (fd < 0 || fd >= MAX_FD || line == NULL)
		return (-1);
	xread = BUFF_SIZE;
	if (tabfd[fd] == NULL)
	{
		if (!(tabfd[fd] = ft_memalloc((BUFF_SIZE + 1) * sizeof(char))))
			return (-1);
		if ((xread = read(fd, tabfd[fd], BUFF_SIZE)) == 0 && tabfd[fd][0] == 0)
		{
			ft_strdel(&tabfd[fd]);
			return (0);
		}
	}
	return (gnl(fd, line, xread, tabfd));
}
