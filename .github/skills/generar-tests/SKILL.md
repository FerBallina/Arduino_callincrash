# Habilidad: Generación Automática de Pruebas

## Disparador / Propósito
Generar pruebas automatizadas robustas cada vez que se cree o modifique un método de negocio.

## Instrucciones de Ejecución
1. Analizar el método provisto por el usuario.
2. Identificar si es lógica de backend (Java/Spring) o flujos de comportamiento (BDD).
3. Generar un archivo de pruebas complementario:
   - Para lógica interna: Crear un test de JUnit 5 utilizando Mockito.
   - Para flujos de negocio/integración: Crear un archivo de escenario en Cucumber (`.feature`).
4. Seguir estrictamente el patrón Arrange-Act-Assert (AAA) en JUnit.
