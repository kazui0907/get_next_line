/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kryochik <kryochik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 17:33:31 by kryochik          #+#    #+#             */
/*   Updated: 2024/06/09 18:16:10 by kryochik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

static char	*extract_line(char *saved, size_t i)
{
	char	*newline;

	if (saved[i] == '\n')
		newline = ft_substr(saved, 0, i + 1);
	else
		newline = ft_strdup(saved);
	return (newline);
}

static char	*get_line(char **saved, int fd)
{
	char	*newline;
	char	*temp;
	size_t	i;

	i = 0;
	while (saved[fd][i] != '\n' && saved[fd][i] != '\0')
		i++;
	newline = extract_line(saved[fd], i);
	if (!newline)
		return (NULL);
	if (saved[fd][i] == '\n')
		temp = ft_strdup(saved[fd] + i + 1);
	else
		temp = NULL;
	free(saved[fd]);
	saved[fd] = temp;
	if (saved[fd] && saved[fd][0] == '\0')
	{
		free(saved[fd]);
		saved[fd] = NULL;
	}
	return (newline);
}

static int	handle_buffer(char **saved, int fd, char *buf)
{
	char	*temp;

	if (!saved[fd])
	{
		saved[fd] = ft_strdup("");
		if (!saved[fd])
		{
			free(buf);
			return (-1);
		}
	}
	temp = ft_strjoin(saved[fd], buf);
	if (!temp)
	{
		free(saved[fd]);
		free(buf);
		return (-1);
	}
	free(saved[fd]);
	saved[fd] = temp;
	return (0);
}

static int	read_line(char **saved, int fd)
{
	char	*buf;
	ssize_t	bytes_read;

	buf = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (-1);
	bytes_read = read(fd, buf, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buf[bytes_read] = '\0';
		if (handle_buffer(saved, fd, buf) == -1)
			return (-1);
		if (ft_strchr(saved[fd], '\n'))
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	if (bytes_read == -1)
	{
		free(saved[fd]);
		saved[fd] = NULL;
	}
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*saved[1024];
	ssize_t		ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	ret = read_line(saved, fd);
	if (ret == -1)
		return (NULL);
	if (ret == 0 && !saved[fd])
		return (NULL);
	line = get_line(saved, fd);
	return (line);
}
