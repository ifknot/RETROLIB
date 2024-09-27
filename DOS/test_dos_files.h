/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      29.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef TEST_DOS_FILES_H
#define TEST_DOS_FILES_H

#include "../TEST/debug_macros.h"

#include "dos_services_types.h"
#include "dos_services_files.h"
#include "dos_error_messages.h"

#include "../MEM/mem.h"

#include <stdio.h>
#include <stdint.h>

namespace test_dos_files {

	void run() {
		INFO("* testing DOS files services...");
		if (YESNO("* 141\ttest disk free space?")) {
			dos::file::disk_space_info_t info;
			dos::get_disk_free_space(0, &info);
			LOG(info.sectors_per_cluster);
			LOG(info.available_clusters);
			LOG(info.bytes_per_sector);
			LOG(info.clusters_per_drive);
			uint32_t total_size = info.bytes_per_sector;
			total_size *= info.sectors_per_cluster;
			total_size *= info.clusters_per_drive;
			uint32_t free_size = info.bytes_per_sector;
			free_size *= info.sectors_per_cluster;
			free_size *= info.available_clusters;
			LOG(free_size);
			LOG(total_size);
			// should error
			dos::get_disk_free_space(9, &info);
			LOG(info.sectors_per_cluster);
			LOG(info.available_clusters);
			LOG(info.bytes_per_sector);
			LOG(info.clusters_per_drive);
		}
		{
			char fpath[13] = "";
			char buffer[255] = "";
			dos::file::handle_t fhandle;
			dos::file::attributes_t fattr;
			uint32_t nbytes = 0;
			dos::file::position_t fpos = 0;
			if (YESNO("* 142\ttest create file?")) {
				INFO("* file create error...");
				LOG(dos::create_file_using_handle(fpath, dos::file::CREATE_READ_WRITE)); // errors out
				INFO("* Enter filename: ");
				LOG(scanf("%s", fpath));
				LOG(dos::create_file_using_handle(fpath, dos::file::CREATE_READ_ONLY | dos::file::CREATE_HIDDEN));
			}
			if (YESNO("* 143\ttest change mode?")) {
				fpath[0] = '\0';
				INFO("* file mode error");
				LOG(dos::get_file_attributes(fpath)); // errors out
				LOG(dos::set_file_attributes(fpath, fattr));
				INFO("* Enter filename: ");
				LOG(scanf("%s", fpath));
				LOG(dos::get_file_attributes(fpath));
				INFO("* Enter attributes: ");
				std::cin >> fattr;
				LOG(dos::set_file_attributes(fpath, fattr));
				LOG(dos::get_file_attributes(fpath));
			}
			if (YESNO("* 144\ttest open file?")) {
				fpath[0] = '\0';
				INFO("* file open error...");
				LOG(dos::open_file_using_handle(fpath, dos::file::ACCESS_READ_ONLY)); // errors out
				INFO("* Enter filename: ");
				INFO(scanf("%s", fpath));
				INFO("dos::open_file_using_handle(fpath, dos::file::ACCESS_READ_WRITE");
				fhandle = dos::open_file_using_handle(fpath, dos::file::ACCESS_READ_WRITE);
				LOG(fhandle);
				LOG(dos::get_file_attributes(fpath));
			}
			if (YESNO("* 145\ttest write file?")) {
				INFO("* Enter text data: ");
				LOG(scanf("%s", buffer));
				LOG(buffer);
				INFO("* Enter nbytes: ");
				std::cin >> nbytes;
				if (nbytes == 255) {
					for (int i = 0; i < 255; ++i) buffer[i] = i;
				}
				LOG(nbytes);
				INFO("* Enter fpos: ");
				std::cin >> fpos;
				LOG(dos::move_file_pointer_using_handle(fhandle, SEEK_END, fpos));
				LOG(dos::write_file_using_handle(fhandle, buffer, nbytes));
			}
			if (YESNO("* 146\ttest read file?")) {
				buffer[0] = '\0';
				mem::address_t addr;
				addr.memloc = (uint32_t)buffer;
				LOG(buffer);
				INFO("* file size");
				LOG(dos::move_file_pointer_using_handle(fhandle, SEEK_SET));
				nbytes = dos::move_file_pointer_using_handle(fhandle, SEEK_END);
				LOG(nbytes);
				INFO("* read entire file");
				LOG(dos::move_file_pointer_using_handle(fhandle, SEEK_SET));
				LOG(dos::read_file_using_handle(fhandle, buffer, nbytes));
				mem::dump(addr, nbytes);
				INFO("* read random from file");
				INFO("* Enter fpos: ");
				std::cin >> fpos;
				LOG(dos::move_file_pointer_using_handle(fhandle, SEEK_SET, fpos));
				INFO("* Enter nbytes: ");
				std::cin >> nbytes;				
				LOG(dos::read_file_using_handle(fhandle, buffer, nbytes));
				mem::dump(addr, nbytes);
			}
			if (YESNO("* 147\ttest close file?")) {
				LOG(dos::close_file_using_handle(fhandle));
				INFO("* file close error...");
				LOG(dos::close_file_using_handle(fhandle)); // errors out
				LOG(dos::move_file_pointer_using_handle(fhandle, SEEK_SET));
			}
			if (YESNO("* 148\ttest delete file?")) {
				fpath[0] = '\0';
				INFO("* file delete error...");
				LOG(dos::delete_file(fpath)); // errors out
				INFO("* Enter filename: ");
				scanf("%s", fpath);
				LOG(dos::delete_file(fpath));
				INFO("* more file delete errors...");
				LOG(dos::get_file_attributes(fpath));
				LOG(dos::delete_file(fpath));
			}
		}
	}

}

#endif