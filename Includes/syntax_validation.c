/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 06:10:34 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/04 06:10:57 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

#ifndef SYNTAX_VALIDATION_H
# define SYNTAX_VALIDATION_H

bool	validate_pipes(t_token_list *tokens);
bool	validate_redirections(t_token_list *tokens);
bool	validate_redirection_end(t_token_list *tokens);
bool	validate_invalid_chars(t_token_list *tokens);
bool	validate_syntax(t_token_list *tokens);
#endif
