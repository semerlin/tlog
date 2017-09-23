#!/bin/bash

# check folder validation
if [ $# -eq 0 ]
then
    echo "usage: $0 <excutable folder>"
    exit 1
fi

# cd into exectuable folder
cd $1

# get test exectuable
files=`ls test_*`

# run test
exec_status=""

run_exec()
{
    ./$1
    err_code=$?
    if [ ${err_code} -ne 0 ]
    then
        exec_status="${exec_status}$1,fail,${err_code}"
    else
        exec_status="${exec_status}$1,success,${err_code}"
    fi
    exec_status="${exec_status}|"
}

for exec_name in ${files}
do
    if [ -x "${exec_name}" ]
    then
        run_exec ${exec_name}
    fi
done

#get name status code max length
max_name_len=0
max_status_len=0
max_code_len=0

IFS="|"
for exec_detail in ${exec_status}
do
    name=`echo ${exec_detail} | cut -d',' -f1`
    status=`echo ${exec_detail} | cut -d',' -f2`
    code=`echo ${exec_detail} | cut -d',' -f3`

    name_len=`expr length ${name}`
    status_len=`expr length ${status}`
    code_len=`expr length ${code}`

    if [ ${name_len} -gt ${max_name_len} ]
    then
        max_name_len=${name_len}
    fi

    if [ ${status_len} -gt ${max_status_len} ]
    then
        max_status_len=${status_len}
    fi

    if [ ${code_len} -gt ${max_code_len} ]
    then
        max_code_len=${code_len}
    fi
done

max_name_len=$(( max_name_len+2 ))
max_status_len=$(( max_status_len+2 ))
max_code_len=$(( max_code_len+2 ))

# output test status
printf "\e[33m %-${max_name_len}s %-${max_status_len}s %-${max_code_len}s\e[0m\n" "name" "status" "code" 
for exec_detail in ${exec_status}
do
    name=`echo ${exec_detail} | cut -d',' -f1`
    status=`echo ${exec_detail} | cut -d',' -f2`
    code=`echo ${exec_detail} | cut -d',' -f3`
    if [ ${status} = "fail" ]
    then
        printf "\e[31m %-${max_name_len}s %-${max_status_len}s %-${max_code_len}s\e[0m\n" ${name} ${status} ${code}
    else
        printf "\e[32m %-${max_name_len}s %-${max_status_len}s %-${max_code_len}s\e[0m\n" ${name} ${status} ${code}
    fi
done


