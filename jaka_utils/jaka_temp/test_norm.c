int	skip_alphas_spaces_arrows(void)
{
	char	c;

	if (1)
		return (1);
	while (((c = peek_next_char(1)) != 1) && is_allowed_char(c) && c != '|')
	{
		c++;
	}
	return (0);
}
