# config.pri

PROJECT_ROOT = $$PWD

CONFIG += debug_and_release

CONFIG(debug, debug|release) {
	message("debug")
	DESTDIR = debug
	OBJECTS_DIR = $$PROJECT_ROOT/build/slim/debug/
	MOC_DIR = $$PROJECT_ROOT/build/slim/debug/moc/

} else {
	message("release")
	DESTDIR = release
	OBJECTS_DIR = $$PROJECT_ROOT/build/slim/release/
	MOC_DIR = $$PROJECT_ROOT/build/slim/release/moc/
}

