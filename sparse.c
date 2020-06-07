#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	if (argc < 2){
		printf("Destination file missing\n");
		
		return 1;
	}
		
	int fd_in = 0;
	int fd_out = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC );
	
	if (fd_out < 0){
		printf("Error opening destination file\n");
		
		return 1;
	}
	
	char buffer[512];
	ssize_t bytes_read;
	int nul_count = 0;
	int total_bytes_read = 0;
	
	while((bytes_read = read(fd_in, buffer, 512))) {
		for (int i = 0; i < bytes_read; i++ ) {
			if (buffer[i] > 0){				
				if (nul_count > 1){
					lseek(fd_out, nul_count, SEEK_CUR);
				} else if (nul_count == 1){
					write(fd_out, "\x00", 1);
				}				
				write(fd_out, buffer + i, 1);
				
				if(nul_count != 0){
					nul_count = 0;
				}
				
			} else{
				nul_count += 1;	
			}
		}		
		total_bytes_read += bytes_read;
	} 	
	
	ftruncate(fd_out, total_bytes_read);	
	close(fd_out);
	
	return 0;
}