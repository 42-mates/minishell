/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:53:12 by alex              #+#    #+#             */
/*   Updated: 2024/07/24 11:55:35 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdio.h>
# include "libft.h"
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include "parser.h"

void executor(t_command *cmd);

#endif