
include ${TOPDIR}/make.rules.lp64

#-------------------------------------------------------------------------------

all : $(LOCAL_OBJ) $(TARGET) $(TARGETS) $(JCE_H) $(JCE_CPP)

#-------------------------------------------------------------------------------

ifeq ($(MFLAGS),all)

$(filter %.a,$(TARGET_32)) : $(LOCAL_OBJ_32) $(REMOTE_OBJ_32)
	ar r $@ $(LOCAL_OBJ_32)
$(filter %.a,$(TARGET_64)) : $(LOCAL_OBJ_64) $(REMOTE_OBJ_64)
	ar r $@ $(LOCAL_OBJ_64)

$(filter %.so,$(TARGET_32)) : $(LOCAL_OBJ_32) $(REMOTE_OBJ_32)
	$(CC) -m32 -shared -o $@ $(LOCAL_OBJ_32)
$(filter %.so,$(TARGET_64)) : $(LOCAL_OBJ_64) $(REMOTE_OBJ_64)
	$(CC) -m64 -shared -o $@ $(LOCAL_OBJ_64)

$(filter-out %.so %.a,$(TARGET_32)) : $(LOCAL_OBJ_32) $(REMOTE_OBJ_32)
	$(CXX) -m32 $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIB_32)
$(filter-out %.so %.a,$(TARGET_64)) : $(LOCAL_OBJ_64) $(REMOTE_OBJ_64)
	$(CXX) -m64 $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIB_64)

$(filter-out %.so %.a %.y,$(TARGETS_32)) : % : %.$(MFLAGS).o
	$(CXX) -m32 $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIB_32)
$(filter-out %.so %.a %.y,$(TARGETS_64)) : % : %.$(MFLAGS).o
	$(CXX) -m64 $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIB_64)

else

$(filter %.a,$(TARGET)) : $(LOCAL_OBJ) $(REMOTE_OBJ)
	ar r $@ $(LOCAL_OBJ)

$(filter %.so,$(TARGET)) : $(LOCAL_OBJ) $(REMOTE_OBJ)
	$(CC) -m$(MFLAGS) -shared -o $@ $(LOCAL_OBJ)

$(filter-out %.so %.a,$(TARGET)) : $(LOCAL_OBJ) $(REMOTE_OBJ)
	$(CXX) -m$(MFLAGS) $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIB)

$(filter-out %.so %.a %.y,$(TARGETS)) : % : %.o
	$(CXX) -m$(MFLAGS) $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIB)

endif

#----------------------------------------------------------------------------------

ifneq ($(JCE_SRC),)
$(JCE_H) $(JCE_CPP) : $(JCE_SRC)
	rm -vf $(JCE_H) $(JCE_CPP) $(JCE_OBJ)
	$(JCE2CPP) ${JCEFLAGS} $(JCE_SRC)

endif
#----------------------------------------------------------------------------------

ifneq ($(HCE_SRC),)
$(JCE_H) $(HCE_CPP) : $(HCE_SRC)
	rm -vf $(HCE_H) $(HCE_CPP) $(HCE_OBJ)
	$(HCE2CPP) $(HCE_SRC)

endif

tar: $(TARGET) $(CONFIG)
	@if [ -d $(TARGET)_tmp_dir ]; then \
		echo "dir has exist:$(TARGET)_tmp_dir, abort."; \
		exit 1; \
	else \
		mkdir $(TARGET)_tmp_dir $(TARGET)_tmp_dir/$(TARGET);\
		cp $(TARGET) $(CONFIG) $(TARGET)_tmp_dir/$(TARGET)/; \
		cd $(TARGET)_tmp_dir; tar cvfz $(TARGET).tgz $(TARGET)/; cd ..; \
		if [ -f "$(TARGET).tgz" ]; then \
			mv -vf $(TARGET).tgz $(TARGET).`date +%Y%m%d%H%M%S`.tgz; \
		fi; \
		mv $(TARGET)_tmp_dir/$(TARGET).tgz ./; \
		rm -rf $(TARGET)_tmp_dir; \
		echo "tar cvfz $(TARGET).tgz ..."; \
	fi
#----------------------------------------------------------------------------------

clean:
	rm -vf $(LOCAL_OBJ) $(TARGET) $(TARGETS) ${CLEANFILE} .*.d.tmp gmon.out

cleanall:
	rm -vf $(LOCAL_OBJ) $(JCE_H) $(JCE_CPP) $(HCE_H) $(HCE_CPP) $(TARGET) $(TARGETS) $(DEP_FILE) ${CLEANFILE} .*.d.tmp .*.d gmon.out core.* *.log

-include $(DEP_FILE)

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
.%.32.d: %.cpp
	@echo "update $@ ..."; \
	echo -n $< | sed s/\.cpp/\.32.o:/ > $@; \
	$(CC) $(INCLUDE) -MM $< | sed '1s/.*.://' >> $@;

%.32.o: %.cpp
	$(CXX) -m32 $(CFLAGS) $(INCLUDE) -o $@ -c $<

.%.32.d: %.c
	@echo "update $@ ..."; \
	echo -n $< | sed s/\.c/\.32.o:/ > $@; \
	$(CC) $(INCLUDE) -MM $< | sed '1s/.*.://' >> $@;

%.32.o: %.c
	$(CC) -m32 $(CFLAGS) $(INCLUDE) -o $@ -c $<

#-------------------------------------------------------------------------------

.%.64.d: %.cpp
	@echo "update $@ ..."; \
	echo -n $< | sed s/\.cpp/\.64.o:/ > $@; \
	$(CC) $(INCLUDE) -MM $< | sed '1s/.*.://' >> $@;

%.64.o: %.cpp
	$(CXX) -m64 $(CFLAGS) $(INCLUDE) -o $@ -c $<

.%.64.d: %.c
	@echo "update $@ ..."; \
	echo -n $< | sed s/\.c/\.64.o:/ > $@; \
	$(CC) $(INCLUDE) -MM $< | sed '1s/.*.://' >> $@;

%.64.o: %.c
	$(CC) -m64 $(CFLAGS) $(INCLUDE) -o $@ -c $<

#----------------------------------------------------------------------------------

.%.d: %.cpp
	@echo "update $@ ..."; \
	echo -n $< | sed s/\.cpp/\.o:/ > $@; \
	$(CC) $(INCLUDE) -MM $< | sed '1s/.*.://' >> $@;

%.o: %.cpp
	$(CXX) -m$(MFLAGS) $(CFLAGS) $(INCLUDE) -o $@ -c $<

.%.d: %.c
	@echo "update $@ ..."; \
	echo -n $< | sed s/\.c/\.o:/ > $@; \
	$(CC) $(INCLUDE) -MM $< | sed '1s/.*.://' >> $@;

%.o: %.c
	$(CC) -m$(MFLAGS) $(CFLAGS) $(INCLUDE) -o $@ -c $<

