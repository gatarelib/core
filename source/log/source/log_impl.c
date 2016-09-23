/*
 *	Logger Library by Parra Studios
 *	Copyright (C) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A generic logger library providing application execution reports.
 *
 */

/* -- Headers -- */

#include <log/log_impl.h>
#include <log/log_handle.h>

#include <log/log_aspect_stream.h>

#include <stdlib.h>

/* -- Member Data -- */

struct log_impl_type
{
	const char * name;
	log_handle handle;
	log_aspect aspects[LOG_ASPECT_SIZE];
};

/* -- Methods -- */

log_aspect_interface log_impl_aspect_interface(enum log_aspect_id aspect_id)
{
	static log_aspect_singleton aspect_impl[LOG_ASPECT_SIZE] =
	{
		NULL, /* &log_aspect_schedule_interface */
		NULL, /* &log_aspect_storage_interface */
		&log_aspect_stream_interface
	};

	return aspect_impl[aspect_id]();
}

log_impl log_impl_create(const char * name)
{
	log_impl impl = malloc(sizeof(struct log_impl_type));

	if (impl != NULL)
	{
		size_t iterator;

		impl->name = name;
		impl->handle = log_handle_create();

		for (iterator = 0; iterator < LOG_ASPECT_SIZE; ++iterator)
		{
			impl->aspects[iterator] = NULL;
		}
	}

	return impl;
}

void log_impl_define(log_impl impl, log_aspect aspect, enum log_aspect_id aspect_id)
{
	impl->aspects[aspect_id] = aspect;
}

const char * log_impl_name(log_impl impl)
{
	return impl->name;
}

log_aspect log_impl_aspect(log_impl impl, enum log_aspect_id aspect_id)
{
	if (aspect_id == LOG_ASPECT_SIZE)
	{
		return NULL;
	}

	return impl->aspects[aspect_id];
}

int log_impl_write(log_impl impl, const char * tag, const log_record_ctor record_ctor)
{
	const log_record record = log_handle_push(impl->handle, record_ctor);

	log_aspect stream_aspect = impl->aspects[LOG_ASPECT_STREAM];

	log_aspect_stream_impl stream_impl = log_aspect_derived(stream_aspect);

	/* TODO */
	if (stream_impl->write(stream_aspect, NULL, 0) != 0)
	{

	}

	/* TODO */
	(void)tag;

	if (record != NULL)
	{
		/* TODO: index record by tag */

		return 0;
	}

	return 1;
}

int log_impl_clear(log_impl impl)
{
	/* TODO: remove all tags associated to records */

	return log_handle_clear(impl->handle);
}

int log_impl_destroy(log_impl impl)
{
	if (impl != NULL)
	{
		int result;

		size_t iterator;

		for (iterator = 0; iterator < LOG_ASPECT_SIZE; ++iterator)
		{
			if (impl->aspects[iterator] != NULL)
			{
				log_aspect_destroy(impl->aspects[iterator]);
			}
		}

		result = log_handle_destroy(impl->handle);

		free(impl);

		return result;
	}

	return 0;
}
