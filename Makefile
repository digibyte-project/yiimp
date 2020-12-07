CC=g++

CPPFLAGS= -g -O2 -DHAVE_CURL -I/usr/include/mysql

# Comment this line to disable address check on login,
# if you use the auto exchange feature...
CPPFLAGS += -DNO_EXCHANGE

LDLIBS=iniparser/libiniparser.a algos/libalgos.a sha3/libhash.a -lpthread -lgmp -lm -lstdc++ -lcurl -lmysqlclient

SOURCES=stratum.cpp db.cpp coind.cpp coind_aux.cpp coind_template.cpp coind_submit.cpp util.cpp list.cpp \
	rpc.cpp job.cpp job_send.cpp job_core.cpp merkle.cpp share.cpp socket.cpp coinbase.cpp \
	client.cpp client_submit.cpp client_core.cpp client_difficulty.cpp remote.cpp remote_template.cpp \
	user.cpp object.cpp json.cpp base58.cpp rpc_curl.cpp

SOURCES+=randomx/aes_hash.cpp \
  randomx/allocator.cpp \
  randomx/argon2_avx2.c \
  randomx/argon2_core.c \
  randomx/argon2_ref.c \
  randomx/argon2_ssse3.c \
  randomx/assembly_generator_x86.cpp \
  randomx/blake2/blake2b.c \
  randomx/blake2_generator.cpp \
  randomx/bytecode_machine.cpp \
  randomx/cpu.cpp \
  randomx/dataset.cpp \
  randomx/instruction.cpp \
  randomx/instructions_portable.cpp \
  randomx/jit_compiler_x86.cpp \
  randomx/jit_compiler_x86_static.S \
  randomx/randomx.cpp \
  randomx/reciprocal.c \
  randomx/soft_aes.cpp \
  randomx/superscalar.cpp \
  randomx/virtual_machine.cpp \
  randomx/virtual_memory.cpp \
  randomx/vm_compiled.cpp \
  randomx/vm_compiled_light.cpp \
  randomx/vm_interpreted.cpp \
  randomx/vm_interpreted_light.cpp

LDCURL = $(shell /usr/bin/pkg-config --static --libs libcurl)

OBJECTS=$(SOURCES:.cpp=.o)
OUTPUT=stratum

CODEDIR1=algos
CODEDIR2=sha3
CODEDIR3=iniparser

.PHONY: projectcode1 projectcode2 projectcode3

all: projectcode1 projectcode2 projectcode3 $(SOURCES) $(OUTPUT)

projectcode1:
	$(MAKE) -C $(CODEDIR1)

projectcode2:
	$(MAKE) -C $(CODEDIR2)

projectcode3:
	$(MAKE) -C $(CODEDIR3)

$(SOURCES): stratum.h util.h

$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDLIBS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f iniparser/*.a
	rm -f iniparser/*.o
	rm -f algos/*.o
	rm -f algos/*.a
	rm -f sha3/*.o
	rm -f sha3/*.a
	rm -f algos/ar2/*.o
	rm -f randomx/*.o

install: clean all
	strip -s stratum
	cp stratum /usr/local/bin/
	cp stratum ../bin/

