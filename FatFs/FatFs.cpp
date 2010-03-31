#include "FatFs.h"
#include <string.h>

FatFsClass FatFs;

FatFsClass::FatFsClass()
{
}

FatFsClass::~FatFsClass()
{
}

#pragma mark FatFs Class Methods

bool FatFsClass::initialize(void)
{
	/* setup sd card slot */
	if(!sd_raw_init()){
		return false;
	}

	/* open first partition */
	_partition = partition_open(sd_raw_read,
								sd_raw_read_interval,
#if SD_RAW_WRITE_SUPPORT
								sd_raw_write,
								sd_raw_write_interval,
#else
								0,
								0,
#endif
								0
								);

	if(!_partition){
		/* If the partition did not open, assume the storage device
		 * is a "superfloppy", i.e. has no MBR.
		 */
		_partition = partition_open(sd_raw_read,
									sd_raw_read_interval,
#if SD_RAW_WRITE_SUPPORT
									sd_raw_write,
									sd_raw_write_interval,
#else
									0,
									0,
#endif
									-1
									);
		if(!_partition){
			return false;
		}
	}

	/* open file system */
	_fs = fat_open(_partition);
	if(!_fs){
		return false;
	}

	/* open root directory */
	return changeDirectory("/");
}

bool FatFsClass::changeDirectory(const char * file_name)
{
	struct fat_dir_entry_struct directory;
	if(fat_get_dir_entry_of_path(_fs, file_name, &directory)){
		fat_close_dir(_dd);
		_dd = fat_open_dir(_fs, &directory);
		if(_dd){
			return true;
		}
	}
	return false;
}

bool FatFsClass::find_file_in_dir(const char *file_name,
								  struct fat_dir_entry_struct *dir_entry)
{
	while(1){
		if(!fat_read_dir(_dd, dir_entry))
			break;
		if(strcmp(file_name, dir_entry->long_name) == 0){
			fat_reset_dir(_dd);
			return true;
		}
	}
	return false;
}

bool FatFsClass::fileExists(const char * file_name)
{
 	struct fat_dir_entry_struct directory;
	return find_file_in_dir(file_name, &directory);
}

#if FAT_WRITE_SUPPORT
bool FatFsClass::deleteDirectory(const char* file_name)
{
	return deleteFile(file_name);
}

bool FatFsClass::createDirectory(const char* file_name)
{
	struct fat_dir_entry_struct directory;
	if(fat_create_dir(_dd, file_name, &directory)){
		sd_raw_sync();
		return true;
	}
	return false;
}

bool FatFsClass::deleteFile(const char * file_name)
{
 	struct fat_dir_entry_struct directory;
	if(find_file_in_dir(file_name, &directory)){
		if(fat_delete_file(_fs, &directory)){
			sd_raw_sync();
			return true;
		}
	}
	return false;
}

bool FatFsClass::createFile(const char * file_name)
{
	struct fat_dir_entry_struct directory;
	if(fat_create_file(_dd, file_name, &directory)){
		sd_raw_sync();
		return true;
	}
	return false;
}
#endif

struct fat_file_struct* FatFsClass::open(const char *file_name)
{
	struct fat_dir_entry_struct directory;
	if(find_file_in_dir(file_name, &directory)){
		return fat_open_file(_fs, &directory);
	}
	return 0;
}

void FatFsClass::finalize(void)
{
	/* close directory */
	fat_close_dir(_dd);

	/* close file system */
	fat_close(_fs);

	/* close partition */
	partition_close(_partition);
}


#pragma mark File Class Methods


File::File()
: _fd(0)
{
}

File::~File()
{
}

bool File::open(const char *file_name)
{
	_fd = FatFs.open(file_name);
	return (_fd != 0);
}

intptr_t File::read(uint8_t* buffer, uintptr_t buffer_len)
{
	return fat_read_file(_fd, buffer, buffer_len);
}

#if FAT_WRITE_SUPPORT
void File::write(uint8_t c)
{
	fat_write_file(_fd, (uint8_t*)&c, 1);
}

void File::write(const char *str)
{
	fat_write_file(_fd, (uint8_t*)str, strlen(str));
}

void File::write(const uint8_t *buffer, size_t size)
{
	fat_write_file(_fd, (uint8_t*)buffer, size);
}

bool File::resize(uint32_t size)
{
	return fat_resize_file(_fd, size);
}

#endif

int32_t File::seekToEnd(int32_t offset)
{
	fat_seek_file(_fd, &offset, FAT_SEEK_END);
}

int32_t File::seekToOffset(int32_t offset)
{
	fat_seek_file(_fd, &offset, FAT_SEEK_SET);
}

void File::close(void)
{
	fat_close_file(_fd);
#if SD_RAW_WRITE_SUPPORT
	sd_raw_sync();
#endif
}
