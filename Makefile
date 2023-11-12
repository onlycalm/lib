Tgt: Log

Log:
	$(MAKE) -C src/c/log

.PHONY: clean

clean:
	$(MAKE) clean -C src/c/log
