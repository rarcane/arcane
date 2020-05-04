TSDEVTERMINAL_COMMAND_PROC(SaveLevelCommand)
{
	if (word_count > 0)
	{
		SaveLevel(words[0]);
	}
	else
	{
		if (core->run_data->current_level[0])
		{
			SaveLevel(core->run_data->current_level);
		}
		else
		{
			LogError("No level is currently active.");
		}
	}

	return 0;
}

TSDEVTERMINAL_COMMAND_PROC(LoadLevelCommand)
{
	if (word_count == 0)
	{
		LogError("Please provide a level name.");
	}
	else
	{
		if (!LoadLevel(words[0]))
			LogError("Failed to load %s", words[0]);
	}

	return 0;
}