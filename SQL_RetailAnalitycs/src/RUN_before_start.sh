mkdir -p export
absolute_path=$(pwd)
    if [ "$(uname)" = "Linux" ] 
    then
        sed 's/\/Users\/hazelnut\/SQL2_Info21_v1.0-0\/src/$absolute_path/' part1.sql
    else
        sed -i '' -E "s!/Users/hazelnut/SQL3_RetailAnalitycs_v1.0-1/src!$absolute_path!" part1.sql
    fi
