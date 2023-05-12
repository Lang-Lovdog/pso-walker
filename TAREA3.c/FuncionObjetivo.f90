module pso_funcion_objetivo
! The update function uses the maximum value, so, if we search for the minimum value,
! then we need to invert the result of FuncionObjetivo.
   use iso_c_binding

contains
   subroutine FuncionObjetivoF(VDP, CDV, PDO, CDP, RDO) bind(C, name="FuncionObjetivoF")
      INTEGER(C_INT), INTENT(IN), VALUE :: CDV
      INTEGER(C_INT), INTENT(IN), VALUE :: CDP
      REAL(C_FLOAT), INTENT(INOUT) :: VDP(CDV)
      REAL(C_FLOAT), INTENT(INOUT) :: PDO(CDP)
      REAL(C_FLOAT), INTENT(INOUT) :: RDO(1)
      !WRITE (*, *) VDP(:)
      !WRITE (*, *) SUM(ABS(VDP(:)))
      !WRITE (*, *) PRODUCT(VDP(:))
      !WRITE (*, *) SUM(ABS(VDP(:)))
      !PRINT *, -(SUM(ABS(VDP(:))) + PRODUCT(VDP(:)))
      !RDO(1) = (SUM(ABS(VDP(:))) + PRODUCT(VDP(:)))
      RDO(1) = maxval(VDP)
   end subroutine
end module
