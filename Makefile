# Detect Platform

ifeq ($(OS), Windows_NT)
    OS_PLATFORM := Windows
else
    OS_PLATFORM := $(shell uname -s)
endif

# OS Specific Configurations

# Windows
ifeq ($(OS_PLATFORM), Windows)
    PY_INTERPRETER:=python
    PY_PIP:=pip
    PY_BUILD_FLAGS=--compiler=mingw32
    FITNESS_TOOL='pylint'
	FORMATTING_TOOL='autopep8'
	TEST_TOOL='pytest'
endif

# Linux
ifeq ($(OS_PLATFORM), Linux)
    PY_INTERPRETER:=python3
    PY_PIP:=pip
    FITNESS_TOOL='pylint'
	FORMATTING_TOOL='autopep8'
	TEST_TOOL='pytest'
endif

MODULE=sia
TEST_DIR=tests
EGG_INFO=python_sia.egg-info
FORMATTING_TOOL_FLAG=--extension-pkg-allow-list=featsat

all:
	@echo $(OS_PLATFORM)
	@echo $(PY_INTERPRETER)
	@echo Do nothing

clean:
	# Cleaning Python execution optimization files
	@find . -name '*.pyc' -delete
	@find . -name '*.pyo' -delete
	@find . -name '__pycache__' -delete
	@rm -rf .pytest_cache

clean_build:
	# Cleaning Extension build directories
	$(PY_INTERPRETER) setup.py clean --all
	rm -rf build
	rm -rf $(EGG_INFO)
	find . -name '*.pyd' -delete
	find . -name '*.so' -delete

score:
	@$(FITNESS_TOOL) $(MODULE) $(FORMATTING_TOOL_FLAG)
	@$(FITNESS_TOOL) $(TEST_DIR) $(FORMATTING_TOOL_FLAG)
	@$(FITNESS_TOOL) setup.py

format:
	@$(FORMATTING_TOOL) -i -r $(MODULE)
	@$(FORMATTING_TOOL) -i -r $(TEST_DIR)
	@$(FORMATTING_TOOL) -i setup.py

test:
	@$(TEST_TOOL)
	#$(TEST_DIR)

build:
	$(PY_INTERPRETER) setup.py build $(PY_BUILD_FLAGS)

install: clean_build build
	$(PY_INTERPRETER) setup.py install $(PY_BUILD_FLAGS)

dev: clean_build build
	$(PY_PIP) install -e .
	# Short installation test
	$(PY_INTERPRETER) test.py