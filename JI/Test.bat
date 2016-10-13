cd release

cd tests
copy ..\Build\Qt*.dll testBinaries
copy ..\Build\icu*.dll testBinaries
run_tests.bat

