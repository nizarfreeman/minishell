#include "minishell.h"

static int	ft_strchr_custom(const char *delim, char c)
{
	while (*delim)
	{
		if (*delim == c)
			return (1);
		delim++;
	}
	return (0);
}

static int count_words(const char *s, const char *delim)
{
	int count = 0, in_quote = 0;
	char quote_char = 0;

	while (*s)
	{
		if ((*s == '\'' || *s == '\"') && (!in_quote || *s == quote_char))
		{
			in_quote = !in_quote;
			quote_char = in_quote ? *s : 0;
			if (!in_quote)
				count++;
		}
		else if (!in_quote && ft_strchr_custom(delim, *s))
		{
			s++;
			continue;
		}
		else if (!in_quote)
		{
			count++;
			while (*s && (!ft_strchr_custom(delim, *s) || in_quote))
			{
				if ((*s == '\'' || *s == '\"') && (!in_quote || *s == quote_char))
					in_quote = !in_quote;
				s++;
			}
			continue;
		}
		s++;
	}
	return (count);
}

static int get_len(const char *s, const char *delim)
{
	int i = 0, in_quote = 0;
	char quote_char = 0;

	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && (!in_quote || s[i] == quote_char))
		{
			in_quote = !in_quote;
			quote_char = in_quote ? s[i] : 0;
		}
		else if (!in_quote && ft_strchr_custom(delim, s[i]))
			break;
		i++;
	}
	return (i);
}

static void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	skip_delim(const char **s, const char *delim)
{
	while (**s && ft_strchr_custom(delim, **s))
		(*s)++;
}

char	**split(const char *s, const char *delim)
{
	char	**res;
	size_t	i = 0, y;
	int	word_count;

	if (!s || !delim)
		return (NULL);
	word_count = count_words(s, delim);
	res = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (*s)
	{
		skip_delim(&s, delim);
		if (!*s)
			break ;
		int	len = get_len(s, delim);
		res[i] = (char *)malloc((len + 1) * sizeof(char));
		if (!res[i])
		{
			free_array(res);
			return (NULL);
		}
		y = 0;
		while (y < len)
			res[i][y++] = *s++;
		res[i++][y] = '\0';
	}
	res[i] = NULL;
	return (res);
}