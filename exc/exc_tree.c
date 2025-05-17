#include "s.h"

int exec_tree(t_tree *root, env **env)
{
	int status;
	if (root->type == WORD || root->type >= 18)
		return (excute(root->args, env, root->fd));
	if (root->type == AND_IF)
	{
		status = exec_tree(root->left, env);
		if(!status)
			return exec_tree(root->right, env);
		return status;
	}
	if (root->type == OR_IF)
	{
		status = exec_tree(root->left, env);
		if(status)
			return exec_tree(root->right, env);
		return status;
	}
	if(root->type == PIPE)
	{
		return exce_pipe(root, env);
	}
	return 0;
}
int pipe_left(t_tree *root, env **env, int  *fds)
{
	// unlink("pipe");
	// int tmp;
	// tmp = dup(STDOUT_FILENO);
	pid_t pid;
	int ex = 1;

	pid = fork();
	if(pid == 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
		ex = exec_tree(root, env);
		exit(ex);
	}
	close(fds[1]);
	return ex;
}
int exce_pipe(t_tree *root, env **env)
{
	int fds[2];
	int ex;
	pipe(fds);

	root->right->fd = fds[0];
	pipe_left(root->left, env, fds);
	ex = exec_tree(root->right, env);
	wait(&ex);
	return ex; 
}