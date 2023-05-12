module CalculoConstriccion
   USE iso_c_binding
contains
   subroutine FuncionObjetivoF(VDP, CDP, RDO) bind(C, NAME="FuncionObjetivoF")
      INTEGER(C_INT), INTENT(IN), VALUE :: CDP
      REAL(C_FLOAT), INTENT(INOUT) :: VDP(CDP)
      REAL(C_FLOAT), INTENT(INOUT) :: RDO(1)
      REAL(C_FLOAT) :: FI
      FI = SUM(VDP(:))
      RDO(1) = 2/ABS(2 - FI - sqrt((FI*FI) - (4*FI)))
   end subroutine
end module
