rm(list = ls())
data = read.csv("datos.csv")
print(data)
library(PolynomF)
library(plotly)
library(rgl)
library(rsm)

#plot3d(data$Col,data$Energy,data$Homo)

sqrt(var(data$Col))/mean(var(data$Col))
sqrt(var(data$Energy))/mean(var(data$Energy))
sqrt(var(data$Contrast))/mean(var(data$Contrast))
sqrt(var(data$Homo))/mean(var(data$Homo))
sqrt(var(data$Entropy))/mean(var(data$Entropy))
sqrt(var(data$IDM))/mean(var(data$IDM))
sqrt(var(data$Mean))/mean(var(data$Mean))

fig <- plot_ly(data, x = ~Col, y = ~Energy, z = ~Homo)
fig <- fig %>% add_markers( marker = list(
  color = 'rgb(0,0,0)',
  size = 2  ))
fig <- fig %>% layout(scene = list(xaxis = list(title = 'Col'),
                                   yaxis = list(title = 'Energy'),
                                   zaxis = list(title = 'Homo')))
fig <- fig %>% add_trace(type = 'mesh3d',
               x = c(0, 0, 150, 150, 0, 0, 150, 150),
               y = c(0, 0.333, 0.333, 0, 0, 0.333, 0.333, 0),
               z = c(0, 0, 0, 0, 1, 1, 1, 1),
               i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
               j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
               k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
               opacity=0.5,
               colorscale=list(c(0,'red')))
fig <- fig %>% add_trace(type = 'mesh3d',
                         x = c(0, 0, 150, 150, 0, 0, 150, 150),
                         y = c(0.333, 0.666, 0.666, 0.333, 0.333, 0.666, 0.666, 0.333),
                         z = c(0.333, 0.333, 0.333, 0.333, 1, 1, 1, 1),
                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
                         j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
                         k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
                         opacity=0.5,
                         color="red")
fig <- fig %>% add_trace(type = 'mesh3d',
                         x = c(0, 0, 150, 150, 0, 0, 150, 150),
                         y = c(0.666, 1, 1, 0.666, 0.666, 1, 1, 0.666),
                         z = c(0.333, 0.333, 0.333, 0.333, 1, 1, 1, 1),
                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
                         j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
                         k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
                         opacity=0.5,
                         color = 'rgb(0, 0, 205)')
fig <- fig %>% add_trace(type = 'mesh3d',
                         x = c(150, 150, 250, 250, 150, 150, 250, 250),
                         y = c(0, 0.5, 0.5, 0, 0, 0.5, 0.5, 0),
                         z = c(0, 0, 0, 0, 0.333, 0.333, 0.333, 0.333),
                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
                         j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
                         k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
                         opacity=0.5,
                         color = 'rgb(0, 0, 205)')
fig <- fig %>% add_trace(type = 'mesh3d',
                         x = c(150, 150, 250, 250, 150, 150, 250, 250),
                         y = c(0, 0.5, 0.5, 0, 0, 0.5, 0.5, 0),
                         z = c(0.333, 0.333, 0.333, 0.333, 1, 1, 1, 1),
                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
                         j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
                         k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
                         opacity=0.5,
                         color = 'rgb(0, 0, 205)')
#fig <- fig %>% add_trace(type = 'mesh3d',
##                         x = c(150, 150, 250, 250, 150, 150, 250, 250),
#                         y = c(0.5, 1, 1, 0.5, 0.5, 1, 1, 0.5),
##                         z = c(0, 0, 0, 0, 0.333, 0.333, 0.333, 0.333),
#                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
 #                        j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
 #                        k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
#                         opacity=0.5,
 #                        color = 'rgb(80, 80, 185)')
fig <- fig %>% add_trace(type = 'mesh3d',
                         x = c(150, 150, 250, 250, 150, 150, 250, 250),
                         y = c(0.5, 1, 1, 0.5, 0.5, 1, 1, 0.5),
                         z = c(0.333, 0.333, 0.333, 0.333, 1, 1, 1, 1),
                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
                         j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
                         k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
                         opacity=0.5,
                         color = 'rgb(0, 0, 205)')
fig <- fig %>% add_trace(type = 'mesh3d',
                         x = c(250, 250, 360, 360, 250, 250, 360, 360),
                         y = c(0, 1, 1, 0, 0, 1, 1, 0),
                         z = c(0, 0, 0, 0, 0.5, 0.5, 0.5, 0.5),
                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
                         j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
                         k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
                         opacity=0.5,
                         color = 'rgb(0, 0, 205)')
fig <- fig %>% add_trace(type = 'mesh3d',
                         x = c(250, 250, 360, 360, 250, 250, 360, 360),
                         y = c(0, 1, 1, 0, 0, 1, 1, 0),
                         z = c(0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1),
                         i = c(7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2),
                         j = c(3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3),
                         k = c(0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6),
                         opacity=0.5,
                         color = 'rgb(100, 100, 100)')
fig


