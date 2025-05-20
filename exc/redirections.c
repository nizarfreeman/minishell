#include "s.h"

void redirections(t_tree *root, env **env, int *ex, t_tree *left)
{
	int fd;
	char **tmp;
	if(root->right->type != HERE_ODC)
	{
		if (root->right->type >= 6 && root->right->type <= 9)
		{
			tmp = pre_expand(root->right->left->args, *env, ex);
			if (tmp && *tmp && tmp[1])
			{
				write(2, "ambiguous redirect\n", 20);
				*ex = 1;
				return ;
			}
			else
			{
				root->right->left->cmd = *tmp;
				*ex = 0;
			}
		}
		else
		{
			tmp = pre_expand(root->right->args, *env, ex);
			if (tmp && *tmp && (tmp[1] || !**tmp))
			{
				write(2, "ambiguous redirect\n", 20);
				*ex = 1;
				return ;
			}
			else
			{
				root->right->cmd = *tmp;
				*ex = 0;
			}
		}
	}
	if (root->type == REDIRECTION_OUT)
	{
		if (root->right->type >= 6 && root->right->type <= 9)
			fd = open(root->right->left->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else
			fd = open(root->right->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd != -1)
		{
			*ex = 0;
			dup2(fd, STDOUT_FILENO);
			if (root->right->type >= 6 && root->right->type <= 9)
			{
				write(2, "here\n", 5);
				redirections(root->right, env, ex, left);
			}
			close(fd);
		}
		else
		{
			perror("open");
			*ex = 1;
		}
		// red_out(root->right, ex);
	}
	else if (root->type == REDIRECTION_IN)
	{
		if (root->right->type >= 6 && root->right->type <= 9)
			fd = open(root->right->left->cmd, O_RDONLY);
		else
			fd = open(root->right->cmd, O_RDONLY);
		if (fd != -1)
		{
			*ex = 0;
			if(left->fd != -1)
				close(left->fd);
			left->fd = fd;
			if (root->right->type >= 6 && root->right->type <= 9)
				redirections(root->right, env, ex, left);
		}
		else
		{
			perror("open");
			*ex = 1;
		}
		// red_in(root->right, ex);
	}
	else if (root->type == APPEND)
	{
		if (root->right->type >= 6 && root->type <= 9)
			fd = open(root->right->left->cmd, O_WRONLY | O_CREAT | O_APPEND);
		else
			fd = open(root->right->cmd, O_WRONLY | O_CREAT | O_APPEND);
		if (fd != -1)
		{
			*ex = 0;
			dup2(fd, STDOUT_FILENO);
			if (root->right->type >= 6 && root->type <= 9)
				redirections(root->right, env, ex, left);
			close(fd);
		}
		else
		{
			perror("open");
			*ex = 1;
		}
		// append(root->right, ex);
	}
	else
	{
		*ex = 0;
		if (root->right->type >= 6 && root->type <= 9)
		{
			if(*(root->right->left->cmd) == '\'' || *(root->right->left->cmd) == '"')
			{
				if(left->fd != -1)
					close(left->fd);
				left->fd = root->fd;
				redirections(root->right, env, ex, left);
			}
			else
			{
				if(left->fd != -1)
					close(left->fd);
				left->fd = read_and_expand(root->fd, *env, ex);	
				redirections(root->right, env, ex, left);
			}
		}
		else
		{
			if(*(root->right->cmd) == '\'' || *(root->right->cmd) == '"')
			{
				if(left->fd != -1)
					close(left->fd);
				left->fd = root->fd;
				// redirections(root->right, env, ex, left);
			}
			else
			{
				if(left->fd != -1)
					close(left->fd);
				left->fd = read_and_expand(root->fd, *env, ex);	
				// redirections(root->right, env, ex, left);
			}
		}

		// printf("%s\n", get_next_line(root->fd));

	}
		// her_doc(root->right, ex);
		return ;
}

int read_and_expand(int fd, env *envr, int *ex)
{
	char  *s = get_next_line(fd);
	int fd_ret;
	env *ret = NULL;
	while(s)
	{
		ft_lstnew(&ret, expand2(s, envr, ex), 0);
		s = get_next_line(fd);
	}
	s = ft_strjoin("/tmp/", ft_itoa((int)&s));
	close(fd);
	unlink(s);
	fd = open(s, O_RDWR);
	fd_ret = open(s, O_RDWR);
	unlink(s);
	while(ret)
	{
		write(fd, ret->value, ft_strlen(ret->value));
		ret = ret->next;
	}
	return fd_ret;
}
