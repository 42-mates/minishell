/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 22:45:36 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/26 00:31:01 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strjoin_char(char *str, char c)
{
    size_t len = ft_strlen(str);
    char *new_str = malloc(len + 2); // +1 для символа c и +1 для нуль-терминатора

    if (!new_str)
        return NULL;

    ft_memcpy(new_str, str, len);
    new_str[len] = c;
    new_str[len + 1] = '\0';
    return new_str;
}

char *extract_variable(char *line, int *i, t_env *env_list, t_shell *shell)
{
    char *var_name;
    char *var_value;

    (*i)++; // Пропускаем символ '$'
    if (line[*i] == '?')
    {
        (*i)++;
        var_value = ft_itoa(shell->exit_status);
        return var_value;
    }
    int start = *i;
    while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
        (*i)++;
    if (start == *i)
        return ft_strdup("$");
    var_name = ft_substr(line, start, *i - start);
    if (!var_name)
        return NULL;
    var_value = getenv_lst(var_name, env_list);
    free(var_name);
    if (var_value)
        return ft_strdup(var_value);
    else
        return ft_strdup("");
}

char *extract_word(char *line, int *i, t_env *env_list, t_shell *shell)
{
    char *value = ft_strdup("");
    if (!value)
        return NULL;
    while (line[*i] && !ft_isspace(line[*i]) && !is_metacharacter(line[*i]))
    {
        if (line[*i] == '$')
        {
            char *var_value = extract_variable(line, i, env_list, shell);
            if (!var_value)
            {
                free(value);
                return NULL; // Обработка ошибки
            }
            char *temp = value;
            value = ft_strjoin(value, var_value);
            free(temp);
            free(var_value);
            if (!value)
                return NULL; // Обработка ошибки
        }
        else
        {
            char *temp = value;
            value = ft_strjoin_char(value, line[*i]);
            free(temp);
            if (!value)
                return NULL; // Обработка ошибки
            (*i)++;
        }
    }

    return value;
}

char *extract_double_quoted_token(char *line, int *i, t_env *env_list, t_shell *shell)
{
    ++(*i); // Пропускаем первую кавычку
    char *value = ft_strdup("");
    if (!value)
        return NULL; // Обработка ошибки выделения памяти

    while (line[*i] && line[*i] != '"')
    {
        if (line[*i] == '$')
        {
            char *var_value = extract_variable(line, i, env_list, shell);
            if (!var_value)
            {
                free(value);
                return NULL; // Обработка ошибки
            }
            char *temp = value;
            value = ft_strjoin(value, var_value);
            free(temp);
            free(var_value);
            if (!value)
                return NULL; // Обработка ошибки
        }
        else
        {
            char *temp = value;
            value = ft_strjoin_char(value, line[*i]);
            free(temp);
            if (!value)
                return NULL; // Обработка ошибки
            (*i)++;
        }
    }

    if (line[*i] == '"')
        (*i)++; // Пропускаем закрывающую кавычку

    return value;
}

char *extract_single_quoted_token(char *line, int *i)
{
    int start = ++(*i); // Пропускаем первую кавычку

    while (line[*i] && line[*i] != '\'')
        (*i)++;

    char *value = ft_substr(line, start, *i - start);
    if (line[*i] == '\'')
        (*i)++; // Пропускаем закрывающую кавычку

    return value;
}