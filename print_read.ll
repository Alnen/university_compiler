; ModuleID = 'nodule'

@0 = private unnamed_addr constant [3 x i8] c"%d\00"
@1 = private unnamed_addr constant [2 x i8] c"\0A\00"
@2 = private unnamed_addr constant [3 x i8] c"%s\00"
@3 = private unnamed_addr constant [15 x i8] c"Enter Z value:\00"
@4 = private unnamed_addr constant [12 x i8] c"Z value is:\00"

define i32 @main() {
"$anonimous1":
  %massive_of_int = alloca [10 x [10 x i32]]
  %massive = alloca [15 x i32]
  %i = alloca i32
  %z = alloca i32
  %mas = alloca [10 x i32]
  %k = alloca i32
  br label %"$anonimous2"

"$anonimous2":                                    ; preds = %"$anonimous1"
  %0 = getelementptr i32, i32* %z
  store i32 5, i32* %0
  %1 = getelementptr i32, i32* %k
  store i32 1, i32* %1
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @2, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @3, i32 0, i32 0))
  %3 = getelementptr i32, i32* %z
  %4 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32* %3)
  %5 = getelementptr i32, i32* %k
  %6 = load i32, i32* %5
  %tempeq = icmp eq i32 %6, 10
  %7 = sext i1 %tempeq to i32
  %8 = trunc i32 %7 to i1
  br i1 %8, label %"$anonimous6", label %"$anonimous10"

exit:                                             ; preds = %"after_if@$anonimous11", %"$anonimous7"
  %9 = getelementptr [10 x i32], [10 x i32]* %mas, i32 5, i32 0
  store i32 1, i32* %9
  ret i32 0

"$anonimous6":                                    ; preds = %"$anonimous2"
  %10 = getelementptr i32, i32* %z
  %11 = load i32, i32* %10
  %templt = icmp slt i32 %11, 5
  %12 = sext i1 %templt to i32
  %13 = trunc i32 %12 to i1
  br i1 %13, label %"$anonimous7", label %"after_if@$anonimous9"

"$anonimous7":                                    ; preds = %"$anonimous6"
  br label %exit

"$anonimous8":                                    ; No predecessors!
  br label %"after_if@$anonimous9"

"after_if@$anonimous9":                           ; preds = %"$anonimous6", %"$anonimous8"
  %14 = getelementptr i32, i32* %i
  store i32 1, i32* %14
  br label %"after_if@$anonimous11"

"$anonimous10":                                   ; preds = %"$anonimous2"
  %15 = getelementptr i32, i32* %z
  %16 = load i32, i32* %15
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @2, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @4, i32 0, i32 0))
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 %16)
  %19 = getelementptr i32, i32* %z
  store i32 1, i32* %19
  br label %"after_if@$anonimous11"

"after_if@$anonimous11":                          ; preds = %"$anonimous10", %"after_if@$anonimous9"
  br label %exit
}

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)
