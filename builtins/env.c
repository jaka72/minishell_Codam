#include "../minishell.h"

int	print_env_export(void)
{
	t_env	*env;

	env = gl.start_env;
	while (env->next)
	{
		printf("declare -x ");
		printf("%s=\"%s\"\n", env->name, env->value);
		env = env->next;
	}
	printf("declare -x ");
	printf("%s=\"%s\"\n", env->name, env->value);
	return (0);
}

int	run_env_builtin(void)
{
	print_env();
	return (0);
}
