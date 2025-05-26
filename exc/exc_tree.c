#include "s.h"

int exec_tree(t_tree *root, env **env, int *ex)
{
	int status;
	if (root->type == WORD || root->type >= 18)
		return (excute(root->args, env, root->fd, ex));
	if (root->type == AND_IF)
	{
		status = exec_tree(root->left, env, ex);
		if(!status)
			return exec_tree(root->right, env, ex);
		return status;
	}
	if (root->type == OR_IF)
	{
		status = exec_tree(root->left, env, ex);
		if(status)
			return exec_tree(root->right, env, ex);
		return status;
	}
	if(root->type == PIPE)
		return exce_pipe(root, env, ex);
	if (root->type >= 6 && root->type <= 9)
	{
		status = fork();
		if (status == 0)
		{
			redirections(root, env, ex, root->left);
			if (!*ex && root->left)
				exec_tree(root->left, env, ex);
			exit(*ex);
		}
		waitpid(status, &status, 0);
		*ex = WEXITSTATUS(status);
	}
	return *ex;
}
int pipe_left(t_tree *root, env **env, int  *fds, int *exi)
{
	pid_t pid;
	int ex = 1;

	pid = fork();
	if(pid == 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
		ex = exec_tree(root, env, exi);
		exit(ex);
	}
	close(fds[1]);
	return pid;
}

int exce_pipe(t_tree *root, env **env, int *exi)
{
	int fds[2];
	int ex;
	pipe(fds);
	int left_pid;

	// root->right->fd = fds[0];
	left_pid = pipe_left(root->left, env, fds, exi);
	int pid = fork();
	if (pid == 0)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		exec_tree(root->right, env, exi);
		exit(*exi);
	}
	close(fds[0]);
	waitpid(left_pid, &ex, 0);
	waitpid(pid, &ex, 0);
	*exi = WEXITSTATUS(ex);
	return ex; 
}