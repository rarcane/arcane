TSDEVTERMINAL_COMMAND_PROC(SaveLevelCommand)
{
	if (word_count > 0)
	{
		SaveLevel(words[0]);
		Log("Successfully saved level to %s.save", words[0]);
	}
	else
	{
		if (core->run_data->current_level[0])
		{
			SaveLevel(core->run_data->current_level);
			Log("Successfully saved level to %s.save", core->run_data->current_level);
		}
		else
		{
			LogError("No level is currently active.");
		}
	}

	return 0;
}