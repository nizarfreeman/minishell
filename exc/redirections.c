#include "s.h"

void redirections(t_tree *root, env **env, int *ex, t_tree *left)
{
	int fd;
	char **tmp;
	if(root->right->type != HERE_ODC)
	{
		if (root->right->type >= 6 && root->right->type <= 9)
		{
			tmp = expand(root->right->left->args, *env, ex);
			if (tmp && *tmp && *tmp[1])
			{
			write(2, "ambiguous redirect\n", 20);
			*ex = 1;
			}
			else
			{
			root->right->left->cmd = *tmp;
			*ex = 0;
			}
		}
		else
		{
			tmp = expand(root->right->args, *env, ex);
		if (tmp && *tmp && *tmp[1])
		{
			write(2, "ambiguous redirect\n", 20);
			*ex = 1;
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
		{
			// printf("%s\n", root->right->cmd);
			fd = open(root->right->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		}
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
		{
			fd = open(root->right->cmd, O_RDONLY);
		}
		if (fd != -1)
		{
			*ex = 0;
			// dup2(fd, STDOUT_FILENO);
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
	}
		// append(root->right, ex);
	else
		return ;
		// her_doc(root->right, ex);
}
