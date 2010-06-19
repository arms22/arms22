#ifndef FatFs_h
#define FatFs_h

#include <Print.h>
#include "byteordering.h"
#include "fat.h"
#include "fat_config.h"
#include "partition.h"
#include "partition_config.h"
#include "sd-reader_config.h"
#include "sd_raw.h"
#include "sd_raw_config.h"


#if FAT_WRITE_SUPPORT
class File : public Print
#else
class File
#endif
{
public:
	File();
	~File();
	bool open(const char *file_name);
	intptr_t read(uint8_t* buffer, uintptr_t buffer_len);
#if FAT_WRITE_SUPPORT
	virtual void write(uint8_t);
	virtual void write(const char *str);
	virtual void write(const uint8_t *buffer, size_t size);
	bool resize(uint32_t size);
#endif
	bool seekToEnd(int32_t offset = 0);
	bool seekToOffset(int32_t offset);
	void close(void);
	
private:
	struct fat_file_struct *_fd;
};


class FatFsClass
{
	friend class File;

public:
	FatFsClass();
	~FatFsClass();
	bool initialize(void);
	bool changeDirectory(const char* file_name);
	bool fileExists(const char* file_name);
#if FAT_WRITE_SUPPORT
	bool deleteDirectory(const char* file_name);
	bool createDirectory(const char* file_name);
	bool deleteFile(const char* file_name);
	bool createFile(const char* file_name);
#endif
	void finalize(void);

private:
	struct partition_struct *_partition;
	struct fat_fs_struct *_fs;
	struct fat_dir_struct *_dd;
	bool find_file_in_dir(const char *file_name,
						  struct fat_dir_entry_struct *dir_entry);
	struct fat_file_struct* open(const char *file_name);
};

extern FatFsClass FatFs;

#endif
