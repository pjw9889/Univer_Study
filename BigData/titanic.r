install.packages("arules")
library("arules")
str(Titanic)

df <- as.data.frame(Titanic)

titanic.raw <- NULL

for(i in 1:4){

  titanic.raw<-cbind(titanic.raw, rep(as.charactere(df[,i])), df$Freq)

}

titanic.raw <- as.data.frame(titanic.raw)
names[titanic.raw] <- names(df)[1:4]

rules <- apriori(titanic.raw, control=list(verbose=F),
parameter = list(minlen=3, supp=0.001, conf=0.1),
appearance=list(rhs=c("Suurvived=Yes"), lhs=c("Class=1st", "Class=2nd",
"Class=3rd", "Sex=Male", "Sex=Female"), default="none"))

quality(rules) <- round(quuality(rules), digits = 3)

rules.sorted <- sort(rules, by="confidence")

inspect(rules.sorted)
