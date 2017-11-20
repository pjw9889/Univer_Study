ms_korea<- userTimeline("MicrosoftKorea", n= 1000)

df<- do.call("rbind", lapply(ms_korea, as.data.frame))

myCorpus<- Corpus(VectorSource(df$text))
myCorpus<- tm_map(myCorpus, tolower)
myCorpus<- tm_map(myCorpus, removePunctuation)
myCorpus<- tm_map(myCorpus, removeNumbers)

removeURL<- function(x) gsub("http[[:alnum:]]*", "", x)
myCorpus<- tm_map(myCorpus, removeURL)

removeAt<- function(x) gsub("@[[:alnum:]]*", "", x)
myCorpus<- tm_map(myCorpus, removeAt)

removeComment<- function(x) gsub("#[[:alnum:]]*", "", x)
myCorpus<- tm_map(myCorpus, removeComment)

removeChar<- function(x) gsub("\u0080[[:alnum:]]*", "", x)
myCorpus<- tm_map(myCorpus, removeChar)

removenoun<- function(x) gsub("있습니다[[:alnum:]]*", "", x)
myCorpus<- tm_map(myCorpus, removenoun)

myCorpus<- tm_map(myCorpus, PlainTextDocument)
tdm<- TermDocumentMatrix(myCorpus, control=list(wordLegnths=c(2,Inf)))

n <- as.matrix(tdm)


wordFreq<- sort(rowSums(n), decreasing=T)
pal <- brewer.pal(8, "Set1")

wordcloud(words=names(wordFreq), freq=wordFreq, scale=c(4,.5), min.freq=20, colors=pal, random.order=F)

wordFreq<- rowSums(n)
wordFreq<- subset(wordFreq, wordFreq>=4)
barplot(wordFreq, las=2)

findAssocs(tdm, c("다시보기", "이매진컵", "클라우드"), 0.3)
