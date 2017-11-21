install.packages("party")
library(party)
library(class)

str(iris) ## iris dataset print

ind <- sample(2, nrow(iris), replace=TRUE, prob=c(0.7, 0.3))
## Save the weight as ind 7: 3

trainData <- iris[ind == 1, 1:4] ## except Species row
testData <- iris[ind == 2, 1:4] ## except Species row

str(trainData) ## trainData set print
str(testData)

class1 <- iris[ind == 1,5] ## trainData set class make
class2 <- iris[ind == 2,5]

length(class1) ## Comparison of the number of data of str function and class
length(class2)

myknn <- knn(trainData, testData, class1, K=3, prob=TRUE)

attributes(.Last.value) ## print attributes about myknn

tab <- table(myknn, class2) ## the result of myknn and class2

tab ## table print

round(sum((tab[row(tab)==col(tab)])/sum(tab)),digit=3)
