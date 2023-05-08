from matplotlib import pyplot as plt
import numpy as np
import math
import random

class fibre:

    def __init__(self,length,angle,centre):
        self.centre = np.array(centre)
        self.angle = math.radians(angle)
        self.length = length
        self.connected = False
        self.inQueue = False

        self.grad = np.array([math.cos(self.angle),math.sin(self.angle)])

        self.grad /= np.sqrt(np.sum(self.grad**2))
        
    def getEnd(self,posEnd):
        if posEnd:
            return self.centre + self.grad*self.length/2
        else:
            return self.centre - self.grad*self.length/2

    def draw(self):
        start = self.centre + (self.length/2)*self.grad
        end = self.centre - (self.length/2)*self.grad
        if self.inQueue:
            colour = "green"
        elif self.connected:
            colour = "red"
        else:
            colour = "blue"

        plt.plot([start[0],end[0]],[start[1],end[1]],color=colour)

class switch:

    def __init__(self,size, numParts, angleLim, partLength, conductDist):
        self.parts = []
        self.conductDist = conductDist
        self.size = size
        for i in range(numParts):
            self.parts.append(fibre(partLength,random.uniform(-angleLim,angleLim),(random.uniform(0,size[0]),random.uniform(0,size[1]))))

    def dist(self,parts,plot=False):

        if self.intersect(parts[0].getEnd(False),parts[0].getEnd(True),parts[1].getEnd(False),parts[1].getEnd(True)):
            return 0

        minDist = math.inf
        minPoints = []

        for i in range(2):
            for j in [False,True]:
                points = (self.nearestLinePointToPoint(parts[i],parts[(i+1)%2].getEnd(j)),parts[(i+1)%2].getEnd(j))
                dist = np.linalg.norm(points[0]-points[1])

                

                if dist < minDist:
                    minDist = dist
                    minPoints = points

        if plot:
            plt.plot((minPoints[0][0],minPoints[1][0]),(minPoints[0][1],minPoints[1][1]))

        return minDist
        

    def nearestLinePointToPoint(self,part,point):
        t = (part.grad[0]*point[0]-part.grad[0]*part.centre[0] + part.grad[1]*point[1]-part.grad[1]*part.centre[1])
        t = np.clip(t,-part.length/2,part.length/2)

        return part.centre + t*part.grad

    def ccw(self,A,B,C):
        return (C[1]-A[1]) * (B[0]-A[0]) > (B[1]-A[1]) * (C[0]-A[0])

    # Return true if line segments AB and CD intersect
    def intersect(self,A,B,C,D):
        return self.ccw(A,C,D) != self.ccw(B,C,D) and self.ccw(A,B,C) != self.ccw(A,B,D)


    def checkConduct(self):
        checkQueue = []

        for i in self.parts:
            if i.centre[0]-i.grad[0]*(i.length/2)-self.conductDist < 0:
                i.connected = True
                i.inQueue = True
                checkQueue.append(i)

        while len(checkQueue) > 0:
            for i in self.parts:
                if i.connected == False:
                    if self.dist((i,checkQueue[-1]),False) < self.conductDist:

                        checkQueue.append(i)
                        i.connected = True
                        i.inQueue = True
                        if i.centre[0]+i.grad[0]*(i.length/2)+self.conductDist > self.size[0]:
                            return True
                        else:
                            break
            else:
                checkQueue[-1].inQueue = False
                checkQueue.pop()
        return False
            


    def drawAll(self):
        for i in self.parts:
            i.draw()

def monteCarlo(tests,iters):
    probs = [0]*len(tests)
    for i in range(len(tests)):
        print("Test num: ",i)
        for j in range(iters):
            test = switch([10,10],32,tests[i],1.5,1)
            if test.checkConduct():
                probs[i] += 1/iters
    return probs


if __name__ == "__main__":
    results = monteCarlo(np.linspace(1,90,20),10000)
    plt.plot(np.linspace(1,90,20),results)
    plt.xlabel("Angle magnitude limit of particles")
    plt.ylabel("Probability of conduction")
    plt.show()
