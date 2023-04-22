$DIC_PATH = "@DIC_PATH@"
$USERDIC_PATH = "@DIC_PATH@\user-dic"
$MECAB_EXEC_PATH = "@BIN_PATH@\mecab.exe"
$DICT_INDEX = "@BIN_PATH@\mecab-dict-index.exe"

function Get-Userdics {
    $Dir = Get-Childitem $USERDIC_PATH -recurse
    $List = $Dir | Where-Object {$_.extension -eq ".csv"}
    $List
}

function Get-Cost {
    $input_dic = $args[0]
    & $DICT_INDEX -m "$($DIC_PATH)/model.def" -d $DIC_PATH -u "$($DIC_PATH)/user-$($input_dic)" -f utf-8 -t utf-8 -a "$($USERDIC_PATH)/$($input_dic)"
}

function Compile {
    Remove-Item "$($DIC_PATH)/*.bin"
    Remove-Item "$($DIC_PATH)/*.txt"
    & $DICT_INDEX -d $DIC_PATH -o $DIC_PATH -f UTF-8 -t UTF-8

}

function main {
    Write-Output "generating userdic..."
    Remove-Item "$($DIC_PATH)/user-*.csv"

    Get-Userdics | ForEach-Object {
        Get-Cost $_.Name
    }


    Compile
}

main

