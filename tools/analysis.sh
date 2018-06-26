#!/bin/bash

# Performs an analysis of Altanal source code:
# - we consider to be in Altanal's source code root
# - we consider having the coverage file altanal_coverage.xml in the root directory
# - we consider having cppcheck, rats, sonar-scanner programs available in the environment

# filter sources:
# - consider generated files in build
# - exclude base *z* files to avoid duplication
# - exclude cblas.h and lapacke-.h because not really part of altanal and make cppcheck analysis too long
./tools/find_sources.sh

# Undefine this because not relevant in our configuration
export UNDEFINITIONS="-UALTANAL_USE_OPENCL -UWIN32 -UWIN64 -U_MSC_EXTENSIONS -U_MSC_VER -U__SUNPRO_C -U__SUNPRO_CC -U__sun -Usun -U__cplusplus"
# run cppcheck analysis
cppcheck -v -f --language=c --platform=unix64 --enable=all --xml --xml-version=2 --suppress=missingInclude ${UNDEFINITIONS} --file-list=./filelist.txt 2> altanal_cppcheck.xml
# run rats analysis
rats -w 3 --xml  `cat filelist.txt` > altanal_rats.xml

# create the sonarqube config file
cat > sonar-project.properties << EOF
sonar.host.url=https://sonarqube.bordeaux.inria.fr/sonarqube
sonar.login=$SONARQUBE_LOGIN

sonar.links.homepage=https://gitlab.inria.fr/solverstack/altanal
sonar.links.scm=https://gitlab.inria.fr/solverstack/altanal.git
sonar.links.ci=https://gitlab.inria.fr/solverstack/altanal/pipelines
sonar.links.issue=https://gitlab.inria.fr/solverstack/altanal/issues

sonar.projectKey=hiepacs:altanal:gitlab:master
sonar.projectDescription=Dense linear algebra subroutines for heterogeneous and distributed architectures
sonar.projectVersion=master

sonar.language=c
sonar.sources=build, compute, control, coreblas, example, include, runtime, testing, timing
sonar.inclusions=`cat filelist.txt | xargs echo | sed 's/ /, /g'`
sonar.c.includeDirectories=$(echo | gcc -E -Wp,-v - 2>&1 | grep "^ " | tr '\n' ',').,include,coreblas/include,runtime/parsec/include,runtime/quark/include,runtime/starpu/include,build,build/control,build/coreblas,build/coreblas/include,build/include,build/runtime/parsec/include,build/runtime/quark/include,build/runtime/starpu/include,hqr/include,$PARSEC_DIR/include,$QUARK_DIR/include,$STARPU_DIR/include/starpu/1.2,$SIMGRID_DIR/include
sonar.sourceEncoding=UTF-8
sonar.c.errorRecoveryEnabled=true
sonar.c.compiler.charset=UTF-8
sonar.c.compiler.parser=GCC
sonar.c.compiler.regex=^(.*):(\\\d+):\\\d+: warning: (.*)\\\[(.*)\\\]$
sonar.c.compiler.reportPath=altanal_starpu.log, altanal_starpu_simgrid.log, altanal_quark.log, altanal_parsec.log
sonar.c.coverage.reportPath=altanal_coverage.xml
sonar.c.cppcheck.reportPath=altanal_cppcheck.xml
sonar.c.rats.reportPath=altanal_rats.xml
sonar.c.clangsa.reportPath=build/analyzer_reports/*/*.plist
EOF

# run sonar analysis + publish on sonarqube-dev
sonar-scanner -X > sonar.log
