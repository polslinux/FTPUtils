all: ftpclient ftpserver ftpclient_gui

ftpclient: bin/ftpclient.c
	@echo "--> Compiling client..."
	@gcc -Wall -O2 -D_FORTIFY_SOURCE=2 -fstack-protector -o ftpclient bin/ftpclient.c
	@echo "--> ...done"
	
ftpclient: bin/ftpserver.c
	@echo "--> Compiling server..."
	@gcc -Wall -O2 -D_FORTIFY_SOURCE=2 -fstack-protector -o ftpserver bin/ftpserver.c bin/list-files.c bin/onexit.c
	@echo "--> ...done"
	
ftpclient_gui: bin/gtk3/entry.c
	@echo "--> Compiling client GUI..."
	@gcc -Wall -O2 -D_FORTIFY_SOURCE=2 -fstack-protector -o ftpclient_gui bin/gtk3/client_ui.c  `pkg-config --cflags --libs gtk+-3.0`
	@echo "--> ...done!"
