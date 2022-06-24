FAULTINJECTORS = ./faultinjectors/fi_read_EINTR.so ./faultinjectors/fi_read_EIO.so \
		 ./faultinjectors/fi_write_ENOSPC.so ./faultinjectors/fi_write_EIO.so \
		 ./faultinjectors/fi_select_ENOMEM.so \
		 ./faultinjectors/fi_malloc_ENOMEM.so

all: $(FAULTINJECTORS)

%.so: %.c
	gcc $< -fPIC -shared -o $@ -ldl

clean:
	rm -f $(FAULTINJECTORS)
