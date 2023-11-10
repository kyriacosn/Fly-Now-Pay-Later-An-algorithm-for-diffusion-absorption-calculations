#READ SIMULATIONS

import numpy as np
import json  


# Give the path to the folder corresponding to the simulation
# The created object contains quantities such as the transition matrices and the midpoints of the bins
# Other information of the simulation are imported through settings (e.g sim.settings["mesh"])
# methods calculate the fluxes (local or global) given a descrete distribution for k (vector)
# each element in vector k corresponds to the bin with the same index
# for example a uniform distribution for a body with 32 bins will be k = np.ones(32)
class simulation_analysis:
    def __init__(self, path):
        
        self.settings = json.load(open(path + "settings.txt", "r"))
        self.t_states = len(self.settings["mesh"]) - 1
        self.trsn_mtrx = np.fromfile(open(path + "transition_mtrx.dat", "r"), dtype = np.double).reshape(self.t_states + 2,self.t_states + 2)
        self.Nij = np.fromfile(open(path + "Nij.dat", "r"), dtype = np.compat.long).reshape(self.t_states + 2,self.t_states + 2)
        self.T = self.trsn_mtrx[:-1,:-1]
        self.pe= self.trsn_mtrx[:-1,-1]
        self.midpoints = np.array(self.settings["mesh"][1:] + np.array(self.settings["mesh"][0:-1]))/2

    def get_flux(self,k):
        p_abs = np.array([0] +list(0.5*np.sqrt(np.pi)*self.settings["LAMBDA"]*k))
        ImQ = np.identity(self.t_states+1) - self.T*((1-p_abs)[:,np.newaxis])   
        R = np.diag(p_abs)
        R[:,0] = self.pe*(1-p_abs)
        return np.linalg.solve(ImQ,R)[0][1:]*self.settings["R0"]
    
    def get_flux_density(self,k):
        return self.get_flux(k)/self.settings["mesh_area"]
    
    def get_total_flux(self,k):
        return np.sum(self.get_flux(k))
    
    def replace_tm(self,Nij):
        tm = np.zeros_like(Nij,dtype = float)
        for i in range(self.t_states + 1):
            tm[i] = Nij[i]/np.sum(Nij[i])
        tm[-1,-1] = 1
        self.trsn_mtrx = tm
        self.T = self.trsn_mtrx[:-1,:-1]
        self.pe= self.trsn_mtrx[:-1,-1]
        
        
    def slsqp_optimize(self,K,k0):
        def wrap_phi(k,self):
            return -self.get_total_flux(k)

        cons = [{"type": "eq", "fun": lambda x: np.ones(self.t_states) @ x - np.array([K*self.t_states])}]
        return minimize(wrap_phi, args= (self), x0=k0, bounds=[(0,K*self.t_states) for i in range(self.t_states)], constraints=cons,method="SLSQP", options={ 'maxiter': 100, 'ftol': 1e-08, 'iprint': 1, 'disp': False, 'eps': 0.01, 'finite_diff_rel_step': None} )
